#include "DB.hpp"

namespace SivImGui::Reflection
{
	DB::DB()
	{
		auto [tpl, _] = m_widgetInfo.emplace(0, WidgetInfo{
			.name = U"WidgetBase",
			.type = typeid(WidgetBase),
			.id = 0,
			.baseId = none,
			.generator = nullptr
		});
		
		detail::RegistrationHelper<WidgetBase>(tpl->second)
			.prop(&SivImGui::WidgetBase::name)
			.prop(&SivImGui::WidgetBase::visible)
			.prop(&SivImGui::WidgetBase::xExpand)
			.prop(&SivImGui::WidgetBase::yExpand)
			.prop(&SivImGui::WidgetBase::enabled)
			.prop(&SivImGui::WidgetBase::enableMouseOver)
			.prop(&SivImGui::WidgetBase::minSize)
			.prop(&SivImGui::WidgetBase::layout)
			.prop(&SivImGui::WidgetBase::gridPos)
			.prop(&SivImGui::WidgetBase::gridSpan);
	}

	HashTable<String, PropertyInfo> DB::getAllProperties(size_t id) const
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
