#pragma once

namespace SivImGui::Reflection::detail
{
	template<class WidgetT>
	class RegistrationHelper
	{
	public:

		RegistrationHelper(WidgetInfo& ref)
			: m_ref(ref) { }

	public:

		template<class BaseT>
		RegistrationHelper& base()
		{
			static_assert(std::is_base_of_v<BaseT, WidgetT>);
			m_ref.baseId = BaseT::TypeInfo().id;
			return *this;
		}

		template<class PropertyT>
		RegistrationHelper& prop(PropertyT WidgetT::* ref, bool textProperty = false)
		{
			m_ref.properties.emplace(PropertyT::Name, PropertyInfo{
				.name = PropertyT::Name,
				.type = typeid(PropertyT::ValueType),
				.flag = PropertyT::Flag,
				.getter = [=](const SivImGui::WidgetBase& base)
				{
					auto& w = dynamic_cast<const WidgetT&>(base);
					return (w.*ref).value();
				},
				.setter = [=](SivImGui::WidgetBase& base, std::any v)
				{
					auto& w = dynamic_cast<WidgetT&>(base);
					(w.*ref) = std::any_cast<PropertyT::ValueType>(v);
				},
			});
			if (textProperty)
			{
				m_ref.textPropertyName = PropertyT::Name;
			}
			return *this;
		}

	private:

		WidgetInfo& m_ref;
	};
}
