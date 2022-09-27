#pragma once
#include "WidgetInfo.hpp"
#include "detail/RegistrationHelper.hpp"

namespace SivImGui::Reflection
{
	class DB
	{
	public:

		DB();

	public:

		template<class WidgetT>
		detail::RegistrationHelper<WidgetT> push()
		{
			const auto& src = WidgetT::TypeInfo();

			auto [itr, success] = m_widgetInfo.try_emplace(src.id, WidgetInfo{
				.name = src.name,
				.type = typeid(WidgetT),
				.id = src.id,
				.baseId = 0,
				.generator = [] { return std::make_unique<WidgetT>(); }
			});
			if (success)
			{
				m_widgetIds.emplace(String{ src.name }, src.id);
			}

			return { itr->second };
		}

		size_t getWidgetId(const StringView widgetName) const
		{
			return m_widgetIds.at(widgetName);
		}

		const WidgetInfo& getWidgetInfo(size_t id) const
		{
			return m_widgetInfo.at(id);
		}

		const WidgetInfo& getWidgetInfo(const StringView name) const
		{
			return m_widgetInfo.at(getWidgetId(name));
		}

		std::unique_ptr<WidgetBase> createInstance(size_t id) const
		{
			return getWidgetInfo(id).generator();
		}

		std::unique_ptr<WidgetBase> createInstance(const StringView name) const
		{
			return createInstance(getWidgetId(name));
		}

		std::unique_ptr<WidgetBase> createInstance(const WidgetInfo& info) const
		{
			return createInstance(info.id);
		}

		HashTable<String, PropertyInfo> getAllProperties(const StringView name) const
		{
			return getAllProperties(getWidgetId(name));
		}

		HashTable<String, PropertyInfo> getAllProperties(const WidgetInfo& info) const
		{
			return getAllProperties(info.id);
		}

		HashTable<String, PropertyInfo> getAllProperties(size_t id) const;

	private:

		HashTable<String, size_t> m_widgetIds;

		std::unordered_map<size_t, WidgetInfo, decltype([](size_t v) { return v; })> m_widgetInfo;
	};
}
