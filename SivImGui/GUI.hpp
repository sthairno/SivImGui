#pragma once
#include <Siv3D.hpp>
#include "Core.hpp"

namespace SivImGui
{
	class GUI
	{
	public:

		GUI(std::unique_ptr<UIElement>&& widget)
		{
			setRootWidget(std::move(widget));
		}

		void setRootWidget(std::unique_ptr<UIElement>&& widget)
		{
			assert(widget);
			m_widget = std::move(widget);
		}

		UIElement& getRootWidget() { return *m_widget; }

		void setEnabled(bool enable) { m_enabled = enable; }

		void layout(Size availableSize = Scene::Size());

		void update(Point pos = { 0, 0 }, Size availableSize = Scene::Size(), bool allowMouseOver = true);

		void draw(Point pos = { 0, 0 }) const;

		template<class WidgetT>
		WidgetT* findWidget(const StringView name = U"")
		{
			return reinterpret_cast<WidgetT*>(findWidget(name, WidgetT::TypeInfo().id));
		}

		UIElement* findWidget(const StringView name, Optional<size_t> typeId = none);

		template<class WidgetT>
		Array<WidgetT*> findAllWidgets(const StringView name = U"")
		{
			return findAllWidgets(name, WidgetT::TypeInfo().id)
				.map([](UIElement* w) { return reinterpret_cast<WidgetT*>(w); });
		}

		Array<UIElement*> findAllWidgets(const StringView name, Optional<size_t> typeId = none);

	private:

		std::unique_ptr<UIElement> m_widget;

		bool m_enabled = true;

		UIElement* m_hoveredWidget = nullptr;
	};
}
