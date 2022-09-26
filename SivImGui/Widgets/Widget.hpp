#pragma once
#include "../Core/WidgetBase.hpp"

namespace SivImGui
{
	class Widget : public WidgetBase
	{
	public:

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

		virtual Size region() const { return { 0, 0 }; };

		virtual WidgetBase* hitTest(Rect rect, Vec2 pos) override
		{
			return hitTestSelf(rect, pos);
		}

		virtual void update(Rect) override { }

		virtual void draw(Rect) const override { }

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

		Array<Optional<Rect>> arrange(Rect) override
		{
			return { };
		};
	};
}
