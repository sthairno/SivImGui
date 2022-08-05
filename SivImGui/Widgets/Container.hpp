#pragma once
#include "WidgetBase.hpp"
#include "Builder.hpp"

namespace SivImGui
{
	class Container : public WidgetBase
	{
		SIVIMGUI_BUILDER_HELPER(Container);

		static Container& New(Builder& ctx)
		{
			return reinterpret_cast<Container&>(ctx.next(1, [] { return new Container(); }));
		}

	public:

		Container() : WidgetBase(1, U"Container", true)
		{ }

	protected:

		Container(WidgetBase::TypeID typeId, StringView typeName)
			: WidgetBase(typeId, typeName, true)
		{ }

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
