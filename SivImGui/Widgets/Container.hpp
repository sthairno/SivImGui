#pragma once
#include "../Core/WidgetBase.hpp"
#include "../Core/Builder.hpp"

namespace SivImGui
{
	class Container : public WidgetBase
	{
		SIVIMGUI_BUILDER_HELPER(Container);

		static Container& New(Builder& ctx)
		{
			return ctx.next<Container>();
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = 1,
				.name = U"Container",
				.enableMouseOver = false
			};
			return info;
		}

	public:

		Container()
			: WidgetBase(TypeInfo(), true) { }

	protected:

		Container(const WidgetTypeInfo& typeInfo)
			: WidgetBase(typeInfo, true) { }

	protected:

		virtual MeasureResult measure() const override
		{
			return std::visit([this](auto& l) {
				return l.measure(visibleChildren());
			}, layout.value());
		}

		virtual Array<Optional<Rect>> arrange(Rect rect) override
		{
			return std::visit([this, rect](auto& l) {
				return l.arrange(rect, visibleChildren());
			}, layout.value());;
		}

		virtual WidgetBase* hitTest(Rect rect, Vec2 pos) override
		{
			if (auto result = hitTestChildren(pos))
			{
				return result;
			}

			return hitTestSelf(rect, pos);
		}

		virtual void update(Rect) override
		{
			updateChildren(visibleChildren());
		}

		virtual void draw(Rect) const override
		{
			drawChildren(visibleChildren());
		}

		template<class ShapeT>
		WidgetBase* hitTestSelf(ShapeT shape, Vec2 pos)
		{
			return
				enableMouseOver->value_or(typeInfo.enableMouseOver) &&
				shape.contains(pos)
				? this : nullptr;
		}

		WidgetBase* hitTestChildren(Vec2 pos)
		{
			for (auto itr = visibleChildren().rbegin();
				itr != visibleChildren().rend();
				itr++)
			{
				if (WidgetBase* result = (*itr)->hitTest(pos))
				{
					return result;
				}
			}

			return nullptr;
		}
	};
}
