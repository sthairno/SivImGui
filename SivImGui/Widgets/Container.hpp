#pragma once
#include "../WidgetBase.hpp"
#include "../Builder.hpp"

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

		virtual Array<RectF> arrange(RectF rect) const override
		{
			return std::visit([this, rect](auto& l) {
				return l.arrange(rect, visibleChildren());
			}, layout.value());;
		}
	};
}
