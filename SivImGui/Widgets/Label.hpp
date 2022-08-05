#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class Label : public Widget
	{
		SIVIMGUI_BUILDER_HELPER(Label);

		static Label& New(Builder& ctx, const String& text)
		{
			auto& w = reinterpret_cast<Label&>(ctx.next(typeid(Label).hash_code(), [&] { return new Label(); }));
			w.text = text;
			return w;
		}

	public:

		Label() : Widget(typeid(Label).hash_code(), U"Label") { }

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
