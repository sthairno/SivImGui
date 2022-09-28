#include "Property.hpp"
#include "UIElement.hpp"

namespace SivImGui::detail
{
	void RequestLayout(UIElement& widget)
	{
		widget.requestLayout();
	}
}
