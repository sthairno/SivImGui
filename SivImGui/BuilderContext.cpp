#include "BuilderContext.hpp"
#include "WidgetCore.hpp"
#include "WidgetController.hpp"

int indent = 0;

namespace GUI::Builder
{
	Context::Context(WidgetCore& root)
		: m_root(&root)
		, m_parent(nullptr)
		, m_last(m_root)
	{
		root.getController().builder = this;
	}

	void Context::reset()
	{
		indent = 0;
		m_parent = nullptr;
		m_last = m_root;
	}

	void Context::pushContainer()
	{
		m_parent = m_last;
		indent++;
		m_last->builderReset();
	}

	void Context::popContainer()
	{
		indent--;
		auto newParent = m_parent->parent();
		if (newParent)
		{
			auto& children = newParent->children();
			if (children.empty())
			{
				m_last = newParent;
			}
			else
			{
				m_last = children.back().get();
			}
		}
		else
		{
			m_last = m_root;
		}
		m_parent = newParent;
	}
}
