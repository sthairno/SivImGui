#pragma once
#include "../Core/UIElement.hpp"

namespace SivImGui
{
	class Container : public UIElement
	{
	public:

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
			: UIElement(TypeInfo(), true) { }

	protected:

		Container(const WidgetTypeInfo& typeInfo)
			: UIElement(typeInfo, true) { }

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

		virtual UIElement* hitTest(Rect rect, Vec2 pos) override
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
		UIElement* hitTestSelf(ShapeT shape, Vec2 pos)
		{
			return
				enableMouseOver->value_or(typeInfo.enableMouseOver) &&
				shape.contains(pos)
				? this : nullptr;
		}

		UIElement* hitTestChildren(Vec2 pos)
		{
			for (auto itr = visibleChildren().rbegin();
				itr != visibleChildren().rend();
				itr++)
			{
				if (UIElement* result = (*itr)->hitTest(pos))
				{
					return result;
				}
			}

			return nullptr;
		}
	};
}
