﻿#include "UIElement.hpp"

namespace SivImGui
{
	void UIElement::requestLayout()
	{
		m_layoutRequired = true;

		if (m_parent)
		{
			m_parent->requestLayout();
		}
	}

	void UIElement::removeChildren()
	{
		m_visibleChildren.clear();
		m_children.clear();

		requestLayout();
	}

	void UIElement::removeChildrenFrom(WidgetContainer::const_iterator first)
	{
		if (first == m_children.cend())
		{
			return;
		}

		m_visibleChildren.erase(
			std::find(m_visibleChildren.cbegin(), m_visibleChildren.cend(), first->get()),
			m_visibleChildren.cend()
		);
		m_children.erase(first, m_children.cend());

		requestLayout();
	}

	void UIElement::addChild(std::unique_ptr<UIElement>&& child)
	{
		assert(isContainer);
		child->m_parent = this;
		m_children.emplace_back(std::move(child));

		requestLayout();
	}

	void UIElement::updateChildren(const WidgetRefContainer& children)
	{
		std::for_each(
			children.rbegin(), children.rend(),
			[=](UIElement* child) { child->updateCore(m_enabled); }
		);
	}

	void UIElement::drawChildren(const WidgetRefContainer& children) const
	{
		std::for_each(
			children.begin(), children.end(),
			[](UIElement* child) { child->drawCore(); }
		);
	}

	void UIElement::updateCore(bool enabled)
	{
		m_enabled = this->enabled && enabled;

		Transformer2D t(Mat3x2::Translate(m_rect.pos), TransformCursor::Yes);
		update({ 0, 0, m_rect.size });
	}

	void UIElement::drawCore() const
	{
		Transformer2D t(Mat3x2::Translate(m_rect.pos), TransformCursor::Yes);
		draw({ 0, 0, m_rect.size });
	}

	void UIElement::measureCore()
	{
		if (not m_layoutRequired)
		{
			return;
		}

		for (auto child : m_visibleChildren)
		{
			child->measureCore();
		}

		m_measuredSize = measure();

		m_measuredSize.minSize.x = Max(m_measuredSize.minSize.x, minSize.value().x);
		m_measuredSize.minSize.y = Max(m_measuredSize.minSize.y, minSize.value().y);
		m_measuredSize.expand.x |= xExpand;
		m_measuredSize.expand.y |= yExpand;
	}

	void UIElement::checkChildrenVisibility()
	{
		m_visibleChildren.clear();
		m_visibleChildren.reserve(m_children.size());
		for (auto& child : m_children)
		{
			bool childVisible = child->visible;
			child->m_visible = childVisible;
			if (childVisible)
			{
				m_visibleChildren.push_back(child.get());
			}
			child->checkChildrenVisibility();
		}
	}

	void UIElement::arrangeCore(Rect rect)
	{
		m_rect.pos = rect.pos;

		if (not m_layoutRequired &&
			m_rect.size == rect.size)
		{
			return;
		}
		m_rect.size = rect.size;

		auto result = arrange({ 0, 0, rect.size });

		assert(result.size() == m_visibleChildren.size());
		for (auto [idx, child] : Indexed(m_visibleChildren))
		{
			if (auto& rect = result[idx])
			{
				child->arrangeCore(*rect);
			}
		}

		m_layoutUpdatedAt = Scene::FrameCount();
		m_layoutRequired = false;
	}
}
