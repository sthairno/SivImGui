#pragma once
#include "TypeDB.hpp"

namespace SivImGui::Reflection
{
	namespace detail
	{
		Texture TextureNameParser(StringView str);

		Font FontNameParser(StringView str);
	}

	class XMLParser
	{
	public:

		XMLParser(const TypeDB& db)
			: m_db(db) {}

	public:

		std::function<Texture(StringView)> textureNameParser = detail::TextureNameParser;

		std::function<Font(StringView)> fontNameParser = detail::FontNameParser;

		std::unique_ptr<UIElement> parse(const XMLElement& elem);

	private:

		const TypeDB& m_db;

		std::any parseProperty(std::type_index type, const StringView str) const;
	};
}
