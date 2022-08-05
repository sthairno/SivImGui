#pragma once
#include <Siv3D.hpp>
#include "Space.hpp"
#include "Layout.hpp"

namespace SivImGui
{
	class Builder;
	class Root;

	class WidgetBase;

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

		Property(WidgetBase& widget, const T defaultValue, PropertyFlag flag = PropertyFlag::None, Validator validator = nullptr)
			: m_value(defaultValue)
			, m_widget(widget)
			, m_flag(flag)
			, m_validator(validator)
		{ }

	public:

		T value() const { return m_value; }

		T& operator =(T value)
		{
			if (value != m_value)
			{
				valueChanged();
				return m_value = value;
			}
			return m_value;
		}

		T& operator*() { return m_value; }

		T* operator->() { return &m_value; }

		const T& operator*() const { return m_value; }

		const T* operator->() const { return &m_value; }

		operator T() const { return m_value; }

	private:

		const PropertyFlag m_flag;

		Validator m_validator;

		WidgetBase& m_widget;

		T m_value;

		void valueChanged()
		{
			if (m_validator)
			{
				assert(m_validator(m_value) && "Validation failed");
			}

			if (m_flag & PropertyFlag::Layout)
			{
				m_widget.requestLayout();
			}
		}
	};

	class WidgetBase
	{
	public:

		using TypeID = size_t;

		using Generator = std::function<WidgetBase* ()>;

		using WidgetContainer = std::vector<std::unique_ptr<WidgetBase>>;

		WidgetBase(TypeID typeId, StringView typeName, bool isContainer)
			: typeId(typeId)
			, typeName(typeName)
			, isContainer(isContainer)
		{ }

	public:

		Property<bool> visible{ *this, true, PropertyFlag::Layout };

		Property<bool> xExpand{ *this, false, PropertyFlag::Layout };

		Property<bool> yExpand{ *this, false, PropertyFlag::Layout };

		Property<SizeF> minSize{ *this, { 0, 0 }, PropertyFlag::Layout };

		Property<Layout> layout{ *this, { HorizontalLayout{} }, PropertyFlag::Layout };

	public:

		const TypeID typeId;

		const StringView typeName;

		const bool isContainer;

		WidgetBase* parent() const { return m_parent; }

		const WidgetContainer& children() const { return m_children; }

		const auto& visibleChildren() const { return m_visibleChildren; }

		RectF rect() const { return m_rect; };

		Vec2 pos() const { return m_rect.pos; };

		SizeF size() const { return m_rect.size; };

		MeasureResult measuredSize() const { return m_measuredSize; };

		void requestLayout() { m_layoutRequired = true; }

		void removeChildren();

		void removeChildrenFrom(WidgetContainer::const_iterator first);

		void addChild(std::unique_ptr<WidgetBase>&& child);

	protected:

		virtual MeasureResult measure() const = 0;

		virtual Array<RectF> arrange(RectF rect) const = 0;

		virtual bool update(RectF, bool) { return false; };

		virtual void draw(RectF) const { }

		void builderPush();

		void builderPop();

	private:

		friend Builder;

		Builder* m_builder = nullptr;

	private:

		friend Root;

		void layoutCore(SizeF availableSize);

		void updateCore(bool& handled);

		void drawCore() const;

	private:

		WidgetBase* m_parent = nullptr;

		WidgetContainer m_children;

		std::vector<WidgetBase*> m_visibleChildren;

		RectF m_rect{ 0, 0, 0, 0 };

		MeasureResult m_measuredSize;

		bool m_visible = true;

		bool m_layoutRequired = true;

		void arrangeCore(RectF rect);

		void measureCore();

		void checkChildrenVisibility();

	public:

		virtual ~WidgetBase() = default;
	};
}
