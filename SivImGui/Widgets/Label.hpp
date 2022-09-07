#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class Label : public Widget
	{
		SIVIMGUI_BUILDER_HELPER(Label);

		static Label& New(Builder& ctx, const StringView text)
		{
			auto& w = ctx.next<Label>();
			w.text = text;
			return w;
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(Label).hash_code(),
				.name = U"Label",
				.enableMouseOver = false
			};
			return info;
		}

	public:

		Label()
			: Widget(TypeInfo()) { }

	public:

		SIVIMGUI_LAYOUT_PROPERTY(String, text, U"");

		SIVIMGUI_LAYOUT_PROPERTY(ColorF, textColor, Palette::White);

		SIVIMGUI_LAYOUT_PROPERTY(Font, font, SimpleGUI::GetFont());

		SIVIMGUI_LAYOUT_PROPERTY(bool, fitHeight, false);

		SIVIMGUI_LAYOUT_PROPERTY(TextStyle, textStyle, TextStyle::Default());

	protected:

		virtual Size region() const override
		{
			return fitHeight ? Size{ 0, 0 } : (*font)(text).region().size.asPoint();
		}

		virtual void draw(Rect rect) const override
		{
			if (fitHeight)
			{
				(*font)(text).draw(textStyle, rect.h, rect.pos, textColor);
			}
			else
			{
				(*font)(text).draw(textStyle, rect.pos, textColor);
			}
		}
	};
}
