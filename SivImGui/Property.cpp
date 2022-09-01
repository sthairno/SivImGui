#include "Property.hpp"
#include "WidgetBase.hpp"

namespace SivImGui::detail
{
	void RequestLayout(WidgetBase& widget)
	{
		widget.requestLayout();
	}
}
