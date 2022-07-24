#pragma once
#include "Common.hpp"
#include "Layout.hpp"
#include "Space.hpp"
#include "Builder.hpp"

namespace GUI
{
	class WidgetCore;

	class WidgetController;

	enum class PropertyFlag
	{
		None = 0,
		Renderer = 0b01,
		Layout = 0b10,
	};
	DEFINE_BITMASK_OPERATORS(PropertyFlag);

	template<class T>
	class Property
	{
		using Validator = std::function<bool(T)>;

	public:

		Property(const T defaultValue, WidgetController& controller, PropertyFlag flag = PropertyFlag::None, Validator validator = nullptr)
			: m_value(defaultValue)
			, m_controller(controller)
			, m_flag(flag)
			, m_validator(validator)
		{ }

	public:

		T value() const { return m_value; }

		T& operator =(const T& value)
		{
			if (value != m_value)
			{
				valueChanged();
				return m_value = value;
			}
			return m_value;
		}

		T& operator =(T&& value)
		{
			if (value != m_value)
			{
				valueChanged();
				return m_value = value;
			}
			return m_value;
		}

		operator T() const { return m_value; }

	private:

		const PropertyFlag m_flag;

		Validator m_validator;

		WidgetController& m_controller;

		T m_value;

		void valueChanged()
		{
			if (m_validator)
			{
				assert(m_validator(m_value) && "Validation failed");
			}

			if (m_flag & PropertyFlag::Layout)
			{
				m_controller.makeDirty();
			}
		}
	};

	class WidgetController
	{
	public:

		WidgetController(WidgetCore& core)
			: core(core)
		{ }

		Builder::Context& getBuilder() { return *builder; }

		void makeDirty();

	public:

		Property<Layout::Variant> layout{ Layout::Default{}, *this, PropertyFlag::Layout };

		Property<bool> sameLine{ false, *this, PropertyFlag::Layout };

		Property<int> row{ 0, *this, PropertyFlag::Layout };

		Property<int> column{ 0, *this, PropertyFlag::Layout };

	protected:

		WidgetCore& core;

		void builderPushContainer();

		void builderPopContainer();

	private:

		friend Builder::Context;

		Builder::Context* builder = nullptr;

	public:

		virtual ~WidgetController()
		{ };

	};
}
