#pragma once
#include "UIElement.hpp"

namespace SivImGui
{
	class Root
	{
	public:

		Root(std::unique_ptr<UIElement>&& widget);

		UIElement& widget() { return *m_widget; }

		UIElement* hoveredWidget() { return m_hoveredWidget; }

		void layout(Point pos, Size availableSize);

		void update(bool enabled = true, bool allowMouseOver = true);

		void draw() const;

	private:

		std::unique_ptr<UIElement> m_widget;

		UIElement* m_hoveredWidget = nullptr;
	};
}
