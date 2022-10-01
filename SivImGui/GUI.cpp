#include "GUI.hpp"

namespace SivImGui
{
	static UIElement* FindWidgetImpl(UIElement* widget, const StringView name, Optional<size_t> typeId)
	{
		if ((not typeId || widget->typeInfo.id == typeId) &&
			widget->name.value() == name)
		{
			return widget;
		}

		for (const auto& child : widget->children())
		{
			if (auto result = FindWidgetImpl(child.get(), name, typeId))
			{
				return result;
			}
		}

		return nullptr;
	}

	static void FindAllWidgetsImpl(UIElement* widget, Array<UIElement*>& result, const StringView name, Optional<size_t> typeId)
	{
		if ((not typeId || widget->typeInfo.id == typeId) &&
			widget->name.value() == name)
		{
			result.push_back(widget);
		}

		for (const auto& child : widget->children())
		{
			FindAllWidgetsImpl(child.get(), result, name, typeId);
		}
	}

	void GUI::layout()
	{
		m_widget->layoutCore(m_availableSize);
	}

	void GUI::update(bool allowMouseOver)
	{
		auto& windowState = Window::GetState();

		if (m_hoveredWidget)
		{
			m_hoveredWidget->m_mouseOver = false;
		}
		allowMouseOver &= not windowState.sizeMove;
		allowMouseOver &= windowState.focused;
		if (allowMouseOver)
		{
			m_hoveredWidget = m_widget->hitTest(Cursor::PosF());
			if (m_hoveredWidget)
			{
				m_hoveredWidget->m_mouseOver = true;
			}
		}

		m_widget->updateCore(m_enabled);
		m_widget->layoutCore(m_availableSize);
	}

	void GUI::draw() const
	{
		m_widget->drawCore();
	}

	UIElement* GUI::findWidget(const StringView name, Optional<size_t> typeId)
	{
		return FindWidgetImpl(m_widget.get(), name, typeId);
	}

	Array<UIElement*> GUI::findAllWidgets(const StringView name, Optional<size_t> typeId)
	{
		Array<UIElement*> result;
		FindAllWidgetsImpl(m_widget.get(), result, name, typeId);
		return result;
	}
}
