﻿#pragma once
#include "../WidgetBase.hpp"
#include "../Builder.hpp"

namespace SivImGui
{
	class Widget : public WidgetBase
	{
		SIVIMGUI_BUILDER_HELPER(Widget)

		static Widget& New(Builder& ctx)
		{
			return ctx.next<Widget&>(TypeInfo());
		}

		static WidgetTypeInfo TypeInfo()
		{
			return {
				.id = 0,
				.name = U"Widget",
				.generator = [] { return std::make_unique<Widget>(); },
				.enableMouseOver = false
			};
		}

	public:

		Widget()
			: WidgetBase(TypeInfo(), false) { }

	protected:

		Widget(WidgetTypeInfo typeInfo)
			: WidgetBase(typeInfo, false) { }

	protected:

		virtual SizeF region() const { return { 0, 0 }; };

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
