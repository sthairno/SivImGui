#include "TypeDB.hpp"

namespace SivImGui::Reflection
{
	TypeDB::TypeDB()
	{
		auto [tpl, _] = m_widgetInfo.emplace(0, WidgetInfo{
			.name = U"UIElement",
			.type = typeid(UIElement),
			.id = 0,
			.baseId = none,
			.generator = nullptr
		});

		detail::RegistrationHelper<UIElement>(tpl->second)
			.prop(&SivImGui::UIElement::name)
			.prop(&SivImGui::UIElement::visible)
			.prop(&SivImGui::UIElement::xExpand)
			.prop(&SivImGui::UIElement::yExpand)
			.prop(&SivImGui::UIElement::enabled)
			.prop(&SivImGui::UIElement::enableMouseOver)
			.prop(&SivImGui::UIElement::minSize)
			.prop(&SivImGui::UIElement::layout)
			.prop(&SivImGui::UIElement::gridPos)
			.prop(&SivImGui::UIElement::gridSpan);
	}

	HashTable<String, PropertyInfo> TypeDB::getAllProperties(size_t id) const
	{
		HashTable<String, PropertyInfo> properties;

		size_t widgetId = id;
		const WidgetInfo* info;

		while (true)
		{
			info = &m_widgetInfo.at(widgetId);
			for (auto& [key, val] : info->properties)
			{
				properties.emplace(key, val);
			}

			if (not info->baseId.has_value())
			{
				break;
			}

			widgetId = *info->baseId;
		}

		return properties;
	}
}
