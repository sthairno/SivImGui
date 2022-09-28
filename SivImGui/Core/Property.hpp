#pragma once
#include "PropertyFlag.hpp"

namespace SivImGui
{
	class UIElement;

	namespace detail
	{
		void RequestLayout(UIElement& widget);

		template<class T, auto GetNameCallback, PropertyFlag _Flag = PropertyFlag::None>
		class Property
		{
		public:

			template<class WidgetT>
			Property(WidgetT* _this, const T defaultValue)
				: m_value(defaultValue)
				, m_widget(*_this)
			{ }

			using ValueType = T;

			constexpr static StringView Name = GetNameCallback();

			constexpr static PropertyFlag Flag = _Flag;

		public:

			const T& value() const { return m_value; }

			template<class U = T> requires std::equality_comparable_with<T, U>
			const T& operator =(U value)
			{
				if (m_value != value)
				{
					if constexpr (Flag & PropertyFlag::Layout)
					{
						detail::RequestLayout(m_widget);
					}
					m_value = value;
				}
				return m_value;
			}

			const T& operator =(T value)
			{
				if (m_value != value)
				{
					if constexpr (Flag & PropertyFlag::Layout)
					{
						detail::RequestLayout(m_widget);
					}
					m_value = value;
				}
				return m_value;
			}

			const T& operator*() const { return m_value; }

			const T* operator->() const { return &m_value; }

			operator T() const { return m_value; }

		private:

			UIElement& m_widget;

			T m_value;
		};
	}
}

#define SIVIMGUI_PROPERTY_TYPE(VALUE_TYPE, NAME, FLAG) \
::SivImGui::detail::Property<VALUE_TYPE, []{ return U ## #NAME; }, FLAG>

#define SIVIMGUI_PROPERTY(VALUE_TYPE, VALUE_NAME, ...) \
SIVIMGUI_PROPERTY_TYPE(VALUE_TYPE, VALUE_NAME, ::SivImGui::PropertyFlag::None) VALUE_NAME{ this, __VA_ARGS__ }

#define SIVIMGUI_LAYOUT_PROPERTY(VALUE_TYPE, VALUE_NAME, ...) \
SIVIMGUI_PROPERTY_TYPE(VALUE_TYPE, VALUE_NAME, ::SivImGui::PropertyFlag::Layout) VALUE_NAME{ this, __VA_ARGS__ }
