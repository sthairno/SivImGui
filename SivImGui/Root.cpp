#include "Root.hpp"

namespace SivImGui
{
	void Root::layout(SizeF availableSize)
	{
		m_widget->layoutCore(availableSize);
	}

	void Root::update(Vec2 pos, bool mouseOver)
	{
		auto& windowState = Window::GetState();
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		mouseOver &= not windowState.sizeMove;
		mouseOver &= windowState.focused;
		m_widget->updateCore(mouseOver, m_enabled);
	}

	void Root::draw(Vec2 pos) const
	{
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->drawCore();
	}
}
