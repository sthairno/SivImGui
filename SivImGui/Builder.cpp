#include "Builder.hpp"

namespace SivImGui
{
	Builder::Builder(WidgetBase& root)
		: m_root(root)
	{
		reset();
	}

	WidgetBase& Builder::nextImpl(const WidgetTypeInfo& info)
	{
		auto& state = m_stack.back();
		auto& children = state.widget->children();

		if (state.nextItr != children.end())
		{
			if (state.nextItr->get()->typeInfo.id == info.id)
			{
				WidgetBase* child = state.nextItr->get();
				state.nextItr++;
				return *child;
			}
			else
			{
				state.widget->removeChildrenFrom(state.nextItr);
			}
		}

		state.widget->addChild(info.generator());
		state.nextItr = children.end();

		WidgetBase& newChild = *children.back();
		newChild.m_builder = this;
		return newChild;
	}

	void Builder::push(WidgetBase& widget)
	{
		m_stack.emplace_back(State{
			.widget = &widget,
			.nextItr = widget.children().begin()
		});
	}

	void Builder::pop()
	{
		auto& state = m_stack.back();
		state.widget->removeChildrenFrom(state.nextItr);
		m_stack.pop_back();
	}

	void Builder::reset()
	{
		m_stack.clear();
		push(m_root);
	}

	void Builder::finalize()
	{
		while (not m_stack.empty())
		{
			pop();
		}
		push(m_root);
	}
}
