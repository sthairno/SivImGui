#include "GUI.hpp"

namespace SivImGui
{
	static WidgetBase* FindWidgetImpl(WidgetBase* widget, const StringView name, Optional<size_t> typeId)
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

	static void FindAllWidgetsImpl(WidgetBase* widget, Array<WidgetBase*>& result, const StringView name, Optional<size_t> typeId)
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

	void GUI::layout(Size availableSize)
	{
		m_widget->layoutCore(availableSize);
	}

	void GUI::update(Point pos, bool allowMouseOver)
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
			m_hoveredWidget = m_widget->hitTest(Cursor::PosF() - pos);
			if (m_hoveredWidget)
			{
				m_hoveredWidget->m_mouseOver = true;
			}
		}

		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->updateCore(m_enabled);
	}

	void GUI::draw(Point pos) const
	{
		Transformer2D t(Mat3x2::Translate(pos), TransformCursor::Yes);
		m_widget->drawCore();
	}

	WidgetBase* GUI::findWidget(const StringView name, Optional<size_t> typeId)
	{
		return FindWidgetImpl(m_widget.get(), name, typeId);
	}

	Array<WidgetBase*> GUI::findAllWidgets(const StringView name, Optional<size_t> typeId)
	{
		Array<WidgetBase*> result;
		FindAllWidgetsImpl(m_widget.get(), result, name, typeId);
		return result;
	}
}
