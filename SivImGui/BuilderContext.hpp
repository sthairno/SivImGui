#pragma once
#include "Common.hpp"
#include "Builder.hpp"
#include "WidgetCore.hpp"

namespace GUI::Builder
{
	class Context
	{
	public:

		Context(WidgetCore& root);

		WidgetCore& getRootWidget() { return *m_root; }

		WidgetCore* getParentWidget() { return m_parent; }

		WidgetCore* getLastWidget() { return m_last; }

		template<class ChildType>
		ChildType& next()
		{
			assert((!m_parent || m_parent->isContainer()) && "This parent is not container");
			ChildType& widget = m_parent->builderNext<ChildType>();
			widget.getController().builder = this;
			m_last = &widget;
			return widget;
		}

		void reset();

		void pushContainer();

		void popContainer();

	private:

		WidgetCore* m_root;

		WidgetCore* m_parent;

		WidgetCore* m_last;
	};
}
