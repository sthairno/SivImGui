#pragma once
#include <Siv3D.hpp>
#include "Property.hpp"
#include "Space.hpp"
#include "Layout.hpp"
#include "WidgetTypeInfo.hpp"

namespace SivImGui
{
	class Root;

	class UIElement
	{
	public:

		using WidgetContainer = std::vector<std::unique_ptr<UIElement>>;

		using WidgetRefContainer = std::vector<UIElement*>;

		UIElement(const WidgetTypeInfo& typeInfo, bool isContainer)
			: typeInfo(typeInfo)
			, isContainer(isContainer)
		{ }

		UIElement(UIElement&) = delete;

	public:

		SIVIMGUI_LAYOUT_PROPERTY(String, name, U"");

		SIVIMGUI_LAYOUT_PROPERTY(bool, visible, true);

		SIVIMGUI_LAYOUT_PROPERTY(bool, xExpand, false);

		SIVIMGUI_LAYOUT_PROPERTY(bool, yExpand, false);

		SIVIMGUI_LAYOUT_PROPERTY(bool, enabled, true);

		SIVIMGUI_LAYOUT_PROPERTY(Optional<bool>, enableMouseOver, { });

		SIVIMGUI_LAYOUT_PROPERTY(Size, minSize, { 0, 0 });

		SIVIMGUI_LAYOUT_PROPERTY(Layout, layout, VerticalLayout{});

		SIVIMGUI_LAYOUT_PROPERTY(Point, gridPos, { 0, 0 });

		SIVIMGUI_LAYOUT_PROPERTY(Size, gridSpan, { 1, 1 });

	public:

		const WidgetTypeInfo& typeInfo;

		const bool isContainer;

		UIElement* parent() const { return m_parent; }

		const WidgetContainer& children() const { return m_children; }

		const WidgetRefContainer& visibleChildren() const { return m_visibleChildren; }

		Rect rect() const { return m_rect; };

		Point pos() const { return m_rect.pos; };

		Size size() const { return m_rect.size; };

		MeasureResult measuredSize() const { return m_measuredSize; };

		bool mouseOver() const { return m_mouseOver; }

		bool isEnabled() const { return m_enabled; }

		int32 layoutUpdatedAt() const { return m_layoutUpdatedAt; }

		UIElement* hitTest(Vec2 pos)
		{
			return hitTest({ 0, 0, m_rect.size }, pos - m_rect.pos);
		}

		void requestLayout();

		void removeChildren();

		void removeChildrenFrom(WidgetContainer::const_iterator first);

		void addChild(std::unique_ptr<UIElement>&& child);

	protected:

		virtual MeasureResult measure() const = 0;

		virtual Array<Optional<Rect>> arrange(Rect rect) = 0;

		virtual UIElement* hitTest(Rect rect, Vec2 pos) = 0;

		virtual void update(Rect rect) = 0;

		virtual void draw(Rect rect) const = 0;

		void updateChildren(const WidgetRefContainer& children);

		void drawChildren(const WidgetRefContainer& children) const;

	private:

		friend Root;

		bool m_mouseOver = false;

		void layoutCore(Size availableSize);

		void updateCore(bool enabled);

		void drawCore() const;

	private:

		UIElement* m_parent = nullptr;

		WidgetContainer m_children;

		WidgetRefContainer m_visibleChildren;

		Rect m_rect{ 0, 0, 0, 0 };

		MeasureResult m_measuredSize;

		bool m_visible = true;

		bool m_layoutRequired = true;

		bool m_enabled = true;

		int32 m_layoutUpdatedAt = 0;

		void arrangeCore(Rect rect);

		void measureCore();

		void checkChildrenVisibility();

	public:

		virtual ~UIElement() = default;
	};
}
