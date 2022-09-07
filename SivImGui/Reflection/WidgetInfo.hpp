#pragma once
#include "PropertyInfo.hpp"

namespace SivImGui::Reflection
{
	struct WidgetInfo
	{
		String name;

		std::type_index type;

		size_t id;

		Optional<size_t> baseId;

		std::function<std::unique_ptr<WidgetBase>()> generator;

		HashTable<String, PropertyInfo> properties;

		std::unique_ptr<WidgetBase> createInstance() const
		{
			return generator();
		}

		const PropertyInfo& getProperty(const String& name) const
		{
			return properties.at(name);
		}
	};
}
