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

		WidgetBase& getWidgetById(const StringView id);

		template<class WidgetT>
		WidgetT& getWidgetById(const StringView id);

	private:

		std::unique_ptr<WidgetBase> m_widget;

		bool m_enabled = true;

		WidgetBase* m_hoveredWidget = nullptr;
	};
}
