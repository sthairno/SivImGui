#include "XMLParser.hpp"
#include "../Util/ParseCSSLikeColor.hpp"

namespace SivImGui::Reflection
{
	Texture detail::TextureNameParser(StringView str)
	{
		if (auto result = UR"(^\s*asset:\s*(\S+)\s*$)"_re.match(str))
		{
			return TextureAsset(result[1]);
		}
		return { };
	}

	Font detail::FontNameParser(StringView str)
	{
		if (auto result = UR"(^\s*asset:\s*(\S+)\s*$)"_re.match(str))
		{
			return FontAsset(result[1]);
		}
		return { };
	}

	static void ParseLayout(UIElement& widget, const XMLElement& e)
	{
		static const HashTable<String, Alignment> str2align = {
			{U"start", Alignment::Start},
			{U"center", Alignment::Center},
			{U"end", Alignment::End},
			{U"stretch", Alignment::Stretch}
		};

		static const auto getPadding = [](const Optional<String>& str) -> Optional<Padding>
		{
			if (str)
			{
				auto list = MathParser(*str).evalArray();
				switch (list.size())
				{
				case 1: return Padding{ static_cast<int32>(list[0]) };
				case 2: return Padding{ static_cast<int32>(list[0]), static_cast<int32>(list[1]) };
				case 3: return Padding{ static_cast<int32>(list[0]), static_cast<int32>(list[1]), static_cast<int32>(list[2]) };
				case 4: return Padding{ static_cast<int32>(list[0]), static_cast<int32>(list[1]), static_cast<int32>(list[2]), static_cast<int32>(list[3]) };
				}
			}
			return none;
		};

		static const auto getAlignment = [](const Optional<String>& str) -> Optional<Alignment>
		{
			if (str)
			{
				auto itr = str2align.find(str->lowercased());
				if (itr != str2align.end())
				{
					return itr->second;
				}
			}
			return none;
		};

		static const auto getSpace = [](const Optional<String>& str) -> Optional<int32>
		{
			if (str)
			{
				return ParseOpt<int32>(*str);
			}
			return none;
		};

		if (e.name() == U"HorizontalLayout")
		{
			widget.layout = HorizontalLayout{
				.padding = getPadding(e.attribute(U"padding")).value_or(Padding{ 0 }),
				.space = getSpace(e.attribute(U"space")).value_or(0),
				.horizontalAlignment = getAlignment(e.attribute(U"horizontalAlignment")).value_or(Alignment::Start),
				.verticalAlignment = getAlignment(e.attribute(U"verticalAlignment")).value_or(Alignment::Stretch),
			};
		}
		else if (e.name() == U"VerticalLayout")
		{
			widget.layout = VerticalLayout{
				.padding = getPadding(e.attribute(U"padding")).value_or(Padding{ 0 }),
				.space = getSpace(e.attribute(U"space")).value_or(0),
				.horizontalAlignment = getAlignment(e.attribute(U"horizontalAlignment")).value_or(Alignment::Stretch),
				.verticalAlignment = getAlignment(e.attribute(U"verticalAlignment")).value_or(Alignment::Start),
			};
		}
		else if (e.name() == U"StackLayout")
		{
			widget.layout = StackLayout{
				.padding = getPadding(e.attribute(U"padding")).value_or(Padding{ 0 }),
				.horizontalAlignment = getAlignment(e.attribute(U"horizontalAlignment")).value_or(Alignment::Stretch),
				.verticalAlignment = getAlignment(e.attribute(U"verticalAlignment")).value_or(Alignment::Stretch),
			};
		}
		else
		{
			throw std::out_of_range(fmt::format("Unknown layout type: {}", e.name().toUTF8()));
		}
	}

	std::unique_ptr<UIElement> XMLParser::parse(const XMLElement& elem)
	{
		auto widget = m_db.getWidgetInfo(elem.name());
		auto properties = m_db.getAllProperties(widget);
		auto instance = widget.createInstance();

		for (auto& [key, value] : elem.attributes())
		{
			try
			{
				auto& property = properties.at(key);
				property.set(*instance, parseProperty(property.type, value));
			}
			catch (std::out_of_range)
			{
				Print << U"Unknown property: \"{}\""_fmt(key);
				// Unknown property
			}
			catch (std::bad_any_cast)
			{
				Print << U"Parse error: {}=\"{}\""_fmt(key, value);
				// Parse error
			}
		}

		if (widget.textPropertyName && elem.text())
		{
			auto& property = properties.at(widget.textPropertyName.value());
			property.set(*instance, parseProperty(property.type, elem.text()));
		}

		XMLElement firstChild = elem.firstChild();
		if (not firstChild.isNull() &&
			firstChild.nextSibling().isNull() &&
			firstChild.name().ends_with(U"Layout"))
		{
			try
			{
				ParseLayout(*instance, firstChild);
			}
			catch (std::out_of_range)
			{
				Print << U"Unknown layout: \"{}\""_fmt(firstChild.name());
			}
			firstChild = firstChild.firstChild();
		}

		for (XMLElement childElem = std::move(firstChild); childElem; childElem = childElem.nextSibling())
		{
			try
			{
				instance->addChild(parse(childElem));
			}
			catch (std::out_of_range)
			{
				Print << U"Unknown widget: \"{}\""_fmt(childElem.name());
			}
		}

		return std::move(instance);
	}

	std::any XMLParser::parseProperty(std::type_index type, const StringView str) const
	{
		static auto opt2any = [](auto v) -> std::any
		{
			return v ? *v : std::any{};
		};

		if (type == typeid(String))
		{
			return String(str);
		}
		else if (type == typeid(ColorF))
		{
			if (auto col = Util::ParseCSSLikeColor(str))
			{
				return *col;
			}
		}
		else if (type == typeid(Point))
		{
			if (auto list = MathParser(str).evalArray())
			{
				if (list.size() == 2)
				{
					return Vec2{ list[0], list[1] }.asPoint();
				}
			}
		}
		else if (type == typeid(Vector2D<double>))
		{
			if (auto list = MathParser(str).evalArray())
			{
				if (list.size() == 2)
				{
					return Vector2D<double>{ list[0], list[1] };
				}
			}
		}
		else if (type == typeid(bool))
		{
			return opt2any(ParseOpt<bool>(str));
		}
		else if (type == typeid(int))
		{
			return opt2any(MathParser(str).evalOpt().map([](double v) { return static_cast<int>(v); }));
		}
		else if (type == typeid(double))
		{
			return opt2any(MathParser(str).evalOpt());
		}
		else if (type == typeid(Texture))
		{
			return textureNameParser ? textureNameParser(str) : Texture{ };
		}
		else if (type == typeid(Font))
		{
			return fontNameParser ? fontNameParser(str) : Font{ };
		}

		return {};
	}
}
