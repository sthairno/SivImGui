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
			setAvailableSize();
			setRootWidget(std::move(widget));
		}

		void setRootWidget(std::unique_ptr<UIElement>&& widget)
		{
			assert(widget);
			m_widget = std::move(widget);
			layout();
		}

		UIElement& getRootWidget() { return *m_widget; }

		bool enabled() const { return m_enabled; }

		Size availableSize() const { return m_availableSize; }

		Size size() const { return m_widget->size(); }

		Size minSize() const { return m_widget->measuredSize().minSize; }

		void setEnabled(bool enable) { m_enabled = enable; }

		void setAvailableSize(Size size = Scene::Size())
		{
			m_availableSize = size;
		}

		void layout();

		void update(bool allowMouseOver = true);

		void draw() const;

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

		Size m_availableSize;

		UIElement* m_hoveredWidget = nullptr;
	};
}
