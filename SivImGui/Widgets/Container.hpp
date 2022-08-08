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
			return ctx.next<Container>(TypeInfo());
		}

		static WidgetTypeInfo TypeInfo()
		{
			return {
				.id = 1,
				.name = U"Container",
				.generator = [] { return std::make_unique<Container>(); }
			};
		}

	public:

		Container()
			: WidgetBase(TypeInfo(), true) { }

	protected:

		Container(WidgetTypeInfo typeInfo)
			: WidgetBase(typeInfo, true) { }

	protected:

		virtual MeasureResult measure() const override
		{
			return layout->measure(visibleChildren());
		}

		virtual Array<RectF> arrange(RectF rect) const override
		{
			return layout->arrange(rect, visibleChildren());
		}
	};
}
