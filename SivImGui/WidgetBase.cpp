#include "WidgetBase.hpp"
#include "Builder.hpp"

namespace SivImGui
{
	void WidgetBase::requestLayout()
	{
		m_layoutRequired = true;

		if (m_parent)
		{
			m_parent->requestLayout();
		}
	}

	void WidgetBase::removeChildren()
	{
		m_visibleChildren.clear();
		m_children.clear();
		
		requestLayout();
	}

	void WidgetBase::removeChildrenFrom(WidgetContainer::const_iterator first)
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

	void WidgetBase::addChild(std::unique_ptr<WidgetBase>&& child)
	{
		assert(isContainer);
		child->m_parent = this;
		m_children.emplace_back(std::move(child));

		requestLayout();
	}

	void WidgetBase::builderPush()
	{
		assert(m_builder);
		m_builder->push(*this);
	}

	void WidgetBase::builderPop()
	{
		assert(m_builder);
		m_builder->pop();
	}

	void WidgetBase::updateChildren(const std::vector<WidgetBase*>& children)
	{
		std::for_each(
			children.rbegin(), children.rend(),
			[=](WidgetBase* child) { child->updateCore(m_enabled); }
		);
	}

	void WidgetBase::drawChildren(const std::vector<WidgetBase*>& children) const
	{
		std::for_each(
			children.begin(), children.end(),
			[](WidgetBase* child) { child->drawCore(); }
		);
	}

	void WidgetBase::layoutCore(SizeF availableSize)
	{
		checkChildrenVisibility();

		measureCore();

		RectF rect{
			0,
			0,
			m_measuredSize.expand.x ? Max(m_measuredSize.minSize.x, availableSize.x) : m_measuredSize.minSize.x,
			m_measuredSize.expand.y ? Max(m_measuredSize.minSize.y, availableSize.y) : m_measuredSize.minSize.y,
		};
		arrangeCore(rect);
	}

	void WidgetBase::updateCore(bool enabled)
	{
		m_enabled = this->enabled && enabled;

		Transformer2D t(Mat3x2::Translate(m_rect.pos), TransformCursor::Yes);
		update(RectF{ 0, 0, m_rect.size });
	}

	void WidgetBase::drawCore() const
	{
		Transformer2D t(Mat3x2::Translate(m_rect.pos), TransformCursor::Yes);
		draw({ 0, 0, m_rect.size });
		
	}

	void WidgetBase::measureCore()
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

	void WidgetBase::checkChildrenVisibility()
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

	void WidgetBase::arrangeCore(RectF rect)
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
			child->arrangeCore(result[idx]);
		}

		m_layoutUpdatedAt = Scene::FrameCount();
		m_layoutRequired = false;
	}
}
