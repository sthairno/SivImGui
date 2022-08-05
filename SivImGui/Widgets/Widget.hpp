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
			return reinterpret_cast<Widget&>(ctx.next(typeid(Widget).hash_code(), [] { return new Widget(); }));
		}

	public:

		Widget() : WidgetBase(0, U"Widget", false)
		{ }

	protected:

		Widget(WidgetBase::TypeID typeId, StringView typeName)
			: WidgetBase(typeId, typeName, false)
		{ }

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
