#include "Builder.hpp"

namespace SivImGui
{
	void detail::BuilderPush(Builder& builder, WidgetBase& widget)
	{
		builder.m_stack.emplace_back(Builder::State{
			.widget = &widget,
			.nextItr = widget.children().begin()
		});
	}

	void detail::BuilderPop(Builder& builder)
	{
		auto& state = builder.m_stack.back();
		state.widget->removeChildrenFrom(state.nextItr);
		builder.m_stack.pop_back();
	}

	Builder::Builder(WidgetBase& root)
		: m_root(root)
	{
		reset();
	}

	WidgetBase& Builder::nextImpl(const WidgetTypeInfo& info, std::function<std::unique_ptr<WidgetBase>()> generator)
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

		state.widget->addChild(generator());
		state.nextItr = children.end();

		WidgetBase& newChild = *children.back();
		return newChild;
	}

	void Builder::reset()
	{
		m_stack.clear();
		detail::BuilderPush(*this, m_root);
	}

	void Builder::finalize()
	{
		while (not m_stack.empty())
		{
			detail::BuilderPop(*this);
		}
		detail::BuilderPush(*this, m_root);
	}
}
