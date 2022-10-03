#include "Root.hpp"

namespace SivImGui
{
	Root::Root(std::unique_ptr<UIElement>&& widget)
	{
		assert(widget);
		m_widget = std::move(widget);
	}

	void Root::layout(Size availableSize)
	{
		m_widget->layoutCore(availableSize);
	}

	void Root::update(bool enabled, bool allowMouseOver)
	{
		if (m_hoveredWidget)
		{
			m_hoveredWidget->m_mouseOver = false;
		}
		if (allowMouseOver)
		{
			m_hoveredWidget = m_widget->hitTest(Cursor::PosF());
			if (m_hoveredWidget)
			{
				m_hoveredWidget->m_mouseOver = true;
			}
		}

		m_widget->updateCore(enabled);
	}

	void Root::draw() const
	{
		m_widget->drawCore();
	}
}
