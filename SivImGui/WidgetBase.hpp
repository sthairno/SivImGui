#pragma once
#include <Siv3D.hpp>
#include "Property.hpp"
#include "Space.hpp"
#include "Layout.hpp"

namespace SivImGui
{
	class Builder;
	class GUI;

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

		Property<String> id{ this, U"" };

		Property<bool> visible{ this, true, PropertyFlag::Layout };

		Property<bool> xExpand{ this, false, PropertyFlag::Layout };

		Property<bool> yExpand{ this, false, PropertyFlag::Layout };

		Property<bool> enabled{ this, true, PropertyFlag::Layout };

		Property<Optional<bool>> enableMouseOver{ this, { }, PropertyFlag::Layout };

		Property<SizeF> minSize{ this, { 0, 0 }, PropertyFlag::Layout };

		Property<Layout> layout{ this, VerticalLayout{}, PropertyFlag::Layout };

		Property<Point> gridPos{ this, { 0, 0 }, PropertyFlag::Layout };

		Property<Size> gridSpan{ this, { 1, 1 }, PropertyFlag::Layout };

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

		int32 layoutUpdatedAt() const { return m_layoutUpdatedAt; }

		WidgetBase* hitTest(Vec2 pos)
		{
			return hitTest(RectF{ 0, 0, m_rect.size }, pos - m_rect.pos);
		}

		void requestLayout();

		void removeChildren();

		void removeChildrenFrom(WidgetContainer::const_iterator first);

		void addChild(std::unique_ptr<WidgetBase>&& child);

	protected:

		virtual MeasureResult measure() const = 0;

		virtual Array<RectF> arrange(RectF rect) = 0;

		virtual WidgetBase* hitTest(RectF rect, Vec2 pos) = 0;

		virtual void update(RectF rect) = 0;

		virtual void draw(RectF rect) const = 0;

		void builderPush();

		void builderPop();

		void updateChildren(const std::vector<WidgetBase*>& children);

		void drawChildren(const std::vector<WidgetBase*>& children) const;

	private:

		friend Builder;

		Builder* m_builder = nullptr;

	private:

		friend GUI;

		bool m_mouseOver = false;

		void layoutCore(SizeF availableSize);

		void updateCore(bool enabled);

		void drawCore() const;

	private:

		WidgetBase* m_parent = nullptr;

		WidgetContainer m_children;

		std::vector<WidgetBase*> m_visibleChildren;

		RectF m_rect{ 0, 0, 0, 0 };

		MeasureResult m_measuredSize;

		bool m_visible = true;

		bool m_layoutRequired = true;

		bool m_enabled = true;

		int32 m_layoutUpdatedAt = 0;

		void arrangeCore(RectF rect);

		void measureCore();

		void checkChildrenVisibility();

	public:

		virtual ~WidgetBase() = default;
	};
}
