#pragma once
#include <Siv3D.hpp>
#include "WidgetBase.hpp"

namespace SivImGui
{
	class GUI
	{
	public:

		GUI(std::unique_ptr<WidgetBase>&& widget)
			: m_widget(std::move(widget))
		{
			assert(m_widget);
		}

		WidgetBase& getRootWidget() { return *m_widget; }

		void setEnabled(bool enable) { m_enabled = enable; }

		void layout(Size availableSize);

		void update(Point pos = { 0, 0 }, bool allowMouseOver = true);

		void draw(Point pos = { 0, 0 }) const;

		template<class WidgetT>
		WidgetT* findWidget(const StringView name = U"")
		{
			return reinterpret_cast<WidgetT*>(getWidget(name, WidgetT::TypeInfo().id));
		}

		WidgetBase* findWidget(const StringView name, Optional<size_t> typeId = none);

		template<class WidgetT>
		Array<WidgetT*> findAllWidgets(const StringView name = U"")
		{
			return findAllWidgets(name, WidgetT::TypeInfo().id)
				.map([](WidgetBase* w) { return reinterpret_cast<WidgetT*>(w); });
		}

		Array<WidgetBase*> findAllWidgets(const StringView name, Optional<size_t> typeId = none);

	private:

		std::unique_ptr<WidgetBase> m_widget;

		bool m_enabled = true;

		WidgetBase* m_hoveredWidget = nullptr;
	};
}
