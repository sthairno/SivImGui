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
	public:

		Property(WidgetBase& widget, const T defaultValue, PropertyFlag flag = PropertyFlag::None)
			: m_value(defaultValue)
			, m_widget(widget)
			, m_flag(flag)
		{ }

	public:

		const T& value() const { return m_value; }

		template<class U = T> requires std::equality_comparable_with<T, U>
		const T& operator =(U value)
		{
			if (m_value != value)
			{
				if (m_flag & PropertyFlag::Layout)
				{
					m_widget.requestLayout();
				}
				return m_value = value;
			}
			return m_value;
		}

		const T& operator =(T value)
		{
			if (m_value != value)
			{
				if (m_flag & PropertyFlag::Layout)
				{
					m_widget.requestLayout();
				}
				return m_value = value;
			}
			return m_value;
		}

		const T& operator*() const { return m_value; }

		const T* operator->() const { return &m_value; }

		operator T() const { return m_value; }

	private:

		const PropertyFlag m_flag;

		WidgetBase& m_widget;

		T m_value;
	};

	struct WidgetTypeInfo
	{
		size_t id;

		StringView name;

		bool enableMouseOver = true;
	};

	class WidgetBase
	{
	public:

		using WidgetContainer = std::vector<std::unique_ptr<WidgetBase>>;

		WidgetBase(const WidgetTypeInfo& typeInfo, bool isContainer)
			: typeInfo(typeInfo)
			, isContainer(isContainer)
		{ }

		WidgetBase(WidgetBase&) = delete;

	public:

		Property<String> id{ *this, U"" };

		Property<bool> visible{ *this, true, PropertyFlag::Layout };

		Property<bool> xExpand{ *this, false, PropertyFlag::Layout };

		Property<bool> yExpand{ *this, false, PropertyFlag::Layout };

		Property<bool> enabled{ *this, true, PropertyFlag::Layout };

		Property<Optional<bool>> enableMouseOver{ *this, { }, PropertyFlag::Layout };

		Property<SizeF> minSize{ *this, { 0, 0 }, PropertyFlag::Layout };

		Property<Layout> layout{ *this, VerticalLayout{}, PropertyFlag::Layout };

		Property<Point> gridPos{ *this, { 0, 0 }, PropertyFlag::Layout };

		Property<Size> gridSpan{ *this, { 1, 1 }, PropertyFlag::Layout };

	public:

		const WidgetTypeInfo& typeInfo;

		const bool isContainer;

		WidgetBase* parent() const { return m_parent; }

		const WidgetContainer& children() const { return m_children; }

		const auto& visibleChildren() const { return m_visibleChildren; }

		RectF rect() const { return m_rect; };

		Vec2 pos() const { return m_rect.pos; };

		SizeF size() const { return m_rect.size; };

		MeasureResult measuredSize() const { return m_measuredSize; };

		bool mouseOver() const { return m_mouseOver; }

		bool isEnabled() const { return m_enabled; }

		void requestLayout() { m_layoutRequired = true; }

		void removeChildren();

		void removeChildrenFrom(WidgetContainer::const_iterator first);

		void addChild(std::unique_ptr<WidgetBase>&& child);

	protected:

		virtual MeasureResult measure() const = 0;

		virtual Array<RectF> arrange(RectF rect) const = 0;

		virtual void update(RectF) { };

		virtual void draw(RectF) const { }

		void builderPush();

		void builderPop();

	private:

		friend Builder;

		Builder* m_builder = nullptr;

	private:

		friend Root;

		void layoutCore(SizeF availableSize);

		bool updateCore(bool mouseOver, bool enabled);

		void drawCore() const;

	private:

		WidgetBase* m_parent = nullptr;

		WidgetContainer m_children;

		std::vector<WidgetBase*> m_visibleChildren;

		RectF m_rect{ 0, 0, 0, 0 };

		MeasureResult m_measuredSize;

		bool m_visible = true;

		bool m_layoutRequired = true;

		bool m_mouseOver = false;

		bool m_enabled = true;

		void arrangeCore(RectF rect);

		void measureCore();

		void checkChildrenVisibility();

	public:

		virtual ~WidgetBase() = default;
	};
}
