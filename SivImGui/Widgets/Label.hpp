#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class Label : public Widget
	{
		SIVIMGUI_BUILDER_HELPER(Label);

		static Label& New(Builder& ctx, const String& text)
		{
			auto& w = ctx.next<Label>(TypeInfo());
			w.text = text;
			return w;
		}

		static WidgetTypeInfo TypeInfo()
		{
			return {
				.id = typeid(Label).hash_code(),
				.name = U"Label",
				.generator = [] { return std::make_unique<Label>(); },
				.enableMouseOver = false
			};
		}

	public:

		Label()
			: Widget(TypeInfo()) { }

	public:

		Property<String> text{ *this, U"", PropertyFlag::Layout };

		Property<ColorF> textColor{ *this, Palette::White, PropertyFlag::Layout };

		Property<Font> font{ *this, SimpleGUI::GetFont(), PropertyFlag::Layout};

	public:

		bool clicked() const { return m_clicked; }

	protected:

		bool m_clicked = false;

		virtual SizeF region() const override
		{
			return (*font)(text).region().size;
		}

		virtual void draw(RectF rect) const override
		{
			(*font)(text).draw(rect.pos, textColor);
		}
	};
}
