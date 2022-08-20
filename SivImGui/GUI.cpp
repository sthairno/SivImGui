#include "GUI.hpp"

namespace SivImGui
{
	void GUI::layout(SizeF availableSize)
	{
		m_widget->layoutCore(availableSize);
	}

	void GUI::update(Vec2 pos, bool allowMouseOver)
	{
		auto& windowState = Window::GetState();

		if (m_hoveredWidget)
		{
			m_hoveredWidget->m_mouseOver = false;
		}
		allowMouseOver &= not windowState.sizeMove;
		allowMouseOver &= windowState.focused;
		if (allowMouseOver)
		{
			m_hoveredWidget = m_widget->hitTest(Cursor::PosF() - pos);
			if (m_hoveredWidget)
			{
				m_hoveredWidget->m_mouseOver = true;
			}
		}

		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->updateCore(m_enabled);
	}

	void GUI::draw(Vec2 pos) const
	{
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->drawCore();
	}
}
