#include "WidgetBase.hpp"
#include "Builder.hpp"

namespace SivImGui
{
	void WidgetBase::removeChildren()
	{
		m_visibleChildren.clear();
		m_children.clear();
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
	}

	void WidgetBase::addChild(std::unique_ptr<WidgetBase>&& child)
	{
		assert(isContainer);
		child->m_parent = this;
		m_children.emplace_back(std::move(child));
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

	void WidgetBase::updateCore(bool& handled)
	{
		Transformer2D t(Mat3x2::Translate(m_rect.pos), TransformCursor::Yes);

		std::for_each(
			m_visibleChildren.rbegin(), m_visibleChildren.rend(),
			[&](WidgetBase* child) { child->updateCore(handled); }
		);
		handled |= update({ 0, 0, m_rect.size }, handled);
	}

	void WidgetBase::drawCore() const
	{
		Transformer2D t(Mat3x2::Translate(m_rect.pos), TransformCursor::Yes);

		draw({ 0, 0, m_rect.size });
		std::for_each(
			m_visibleChildren.begin(), m_visibleChildren.end(),
			[](WidgetBase* child) { child->drawCore(); }
		);
	}

	void WidgetBase::measureCore()
	{
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
		m_rect = rect;

		auto result = arrange({ 0, 0, rect.size });

		assert(result.size() == m_visibleChildren.size());
		for (auto [idx, child] : Indexed(m_visibleChildren))
		{
			child->arrangeCore(result[idx]);
		}
	}
}
