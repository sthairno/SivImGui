#include "Root.hpp"

namespace SivImGui
{
	void Root::layout(SizeF availableSize)
	{
		m_widget->layoutCore(availableSize);
	}

	bool Root::update(Vec2 pos)
	{
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		bool handled = false;
		m_widget->updateCore(handled);
		return handled;
	}

	void Root::draw(Vec2 pos) const
	{
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->drawCore();
	}
}
