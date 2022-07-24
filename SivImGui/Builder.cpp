#include "Builder.hpp"
#include "BuilderContext.hpp"
#include "WidgetController.hpp"

namespace GUI::Builder
{
	void SetLayout(Context& ctx, Layout::Variant&& layout)
	{
		assert(ctx.getParentWidget());
		ctx.getParentWidget()->getController().layout = layout;
	}

	void SameLine(Context& ctx, bool value)
	{
		const auto last = ctx.getLastWidget();
		const auto parent = ctx.getParentWidget();
		if (last && last != parent)
		{
			last->getController().sameLine = value;
		}
	}
};
