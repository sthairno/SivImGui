#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class SimpleButton : public Widget
	{
	public:

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(SimpleButton).hash_code(),
				.name = U"SimpleButton",
			};
			return info;
		}

	public:

		SimpleButton()
			: Widget(TypeInfo()) { }

	public:

		SIVIMGUI_LAYOUT_PROPERTY(String, text, U"");

	public:

		bool clicked() const { return m_clicked; }

	protected:

		bool m_clicked = false;

		virtual Size region() const override
		{
			return SimpleGUI::ButtonRegion(*text, { 0, 0 }).size.asPoint();
		}

		virtual void update(Rect rect) override
		{
			const Rect btnRect(rect.pos, rect.w, measuredSize().minSize.y);

			m_clicked = mouseOver() && btnRect.leftClicked();
		}

		virtual void draw(Rect rect) const override
		{
			[[maybe_unused]]
			bool _ = SimpleGUI::Button(*text, rect.pos, rect.w, isEnabled());
		}
	};
}
