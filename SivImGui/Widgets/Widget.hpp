#pragma once
#include "../WidgetBase.hpp"
#include "../Builder.hpp"

namespace SivImGui
{
	class Widget : public WidgetBase
	{
		SIVIMGUI_BUILDER_HELPER(Widget)

		static Widget& New(Builder& ctx)
		{
			return ctx.next<Widget>();
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = 0,
				.name = U"Widget",
				.enableMouseOver = false
			};
			return info;
		}

	public:

		Widget()
			: WidgetBase(TypeInfo(), false) { }

	protected:

		Widget(const WidgetTypeInfo& typeInfo)
			: WidgetBase(typeInfo, false) { }

	protected:

		virtual SizeF region() const { return { 0, 0 }; };

		virtual WidgetBase* hitTest(RectF rect, Vec2 pos) override
		{
			return hitTestSelf(rect, pos);
		}

		virtual void update(RectF) override { }

		virtual void draw(RectF) const override { }

		template<class ShapeT>
		WidgetBase* hitTestSelf(ShapeT shape, Vec2 pos)
		{
			return
				enableMouseOver->value_or(typeInfo.enableMouseOver) &&
				shape.contains(pos)
				? this : nullptr;
		}

	private:

		MeasureResult measure() const override
		{
			return MeasureResult{ .minSize = region(), .expand = { false, false } };
		}

		Array<RectF> arrange(RectF) const override
		{
			return { };
		};
	};
}
