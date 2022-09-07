#include "DB.hpp"

namespace SivImGui::Reflection
{
	DB::DB()
	{
		m_widgetInfo.emplace(0, WidgetInfo{
			.name = U"WidgetBase",
			.type = typeid(WidgetBase),
			.id = 0,
			.baseId = none,
			.generator = nullptr
		});
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
