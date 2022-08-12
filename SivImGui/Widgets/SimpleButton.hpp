#pragma once
#include "Widget.hpp"

namespace SivImGui
{
	class SimpleButton : public Widget
	{
		SIVIMGUI_BUILDER_HELPER(SimpleButton);

		static SimpleButton& New(Builder& ctx, const String& label)
		{
			auto& w = ctx.next<SimpleButton>();
			w.label = label;
			return w;
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(SimpleButton).hash_code(),
				.name = U"SimpleButton",
				.generator = [] { return std::make_unique<SimpleButton>(); }
			};
			return info;
		}

	public:

		SimpleButton()
			: Widget(TypeInfo()) { }

	public:

		Property<String> label{ *this, U"", PropertyFlag::Layout };

	public:

		bool clicked() const { return m_clicked; }

	protected:

		bool m_clicked = false;

		virtual SizeF region() const override
		{
			return SimpleGUI::ButtonRegion(label, { 0, 0 }).size;
		}

		virtual void update(RectF rect) override
		{
			const RectF btnRect(rect.pos, rect.w, measuredSize().minSize.y);

			m_clicked = mouseOver() && btnRect.leftClicked();
		}

		virtual void draw(RectF rect) const override
		{
			[[maybe_unused]]
			bool _ = SimpleGUI::Button(label, rect.pos, rect.w, isEnabled());
		}
	};
}
