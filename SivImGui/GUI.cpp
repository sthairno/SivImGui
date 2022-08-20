#include "GUI.hpp"

namespace SivImGui
{
	void GUI::layout(SizeF availableSize)
	{
		m_widget->layoutCore(availableSize);
	}

	void GUI::update(Vec2 pos, bool mouseOver)
	{
		auto& windowState = Window::GetState();
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		mouseOver &= not windowState.sizeMove;
		mouseOver &= windowState.focused;
		m_widget->updateCore(mouseOver, m_enabled);
	}

	void GUI::draw(Vec2 pos) const
	{
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->drawCore();
	}
}
