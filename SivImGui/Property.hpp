#pragma once

namespace SivImGui
{
	class WidgetBase;

	namespace detail
	{
		void RequestLayout(WidgetBase& widget);
	}

	enum class PropertyFlag
	{
		None = 0,
		Renderer = 0b01,
		Layout = 0b10,
	};
	DEFINE_BITMASK_OPERATORS(PropertyFlag);

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

		WidgetBase& m_widget;

		T m_value;
	};
}

#define SIVIMGUI_PROPERTY_TYPE(VALUE_TYPE, NAME, FLAG) \
::SivImGui::Property<VALUE_TYPE, []{ return U ## #NAME; }, FLAG>

#define SIVIMGUI_PROPERTY(VALUE_TYPE, VALUE_NAME, ...) \
SIVIMGUI_PROPERTY_TYPE(VALUE_TYPE, VALUE_NAME, ::SivImGui::PropertyFlag::None) VALUE_NAME{ this, __VA_ARGS__ }

#define SIVIMGUI_LAYOUT_PROPERTY(VALUE_TYPE, VALUE_NAME, ...) \
SIVIMGUI_PROPERTY_TYPE(VALUE_TYPE, VALUE_NAME, ::SivImGui::PropertyFlag::Layout) VALUE_NAME{ this, __VA_ARGS__ }
