#include "Root.hpp"

namespace SivImGui
{
	Root::Root(std::unique_ptr<UIElement>&& widget)
	{
		assert(widget);
		m_widget = std::move(widget);
	}

	void Root::layout(Point pos, Size availableSize)
	{
		m_widget->checkChildrenVisibility();

		m_widget->measureCore();
		const MeasureResult measuredSize = m_widget->measuredSize();
		Rect rect{
			pos,
			measuredSize.expand.x ? Max(measuredSize.minSize.x, availableSize.x) : measuredSize.minSize.x,
			measuredSize.expand.y ? Max(measuredSize.minSize.y, availableSize.y) : measuredSize.minSize.y,
		};
		m_widget->arrangeCore(rect);
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
