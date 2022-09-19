#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class TabView : public Container
	{
		SIVIMGUI_BUILDER_HELPER(TabView);

	public:

		static TabView& New(Builder& ctx)
		{
			return ctx.next<TabView>();
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(TabView).hash_code(),
				.name = U"TabView",
				.enableMouseOver = true
			};
			return info;
		}

	public:

		TabView()
			: Container(TypeInfo()) { }

	public:

		SIVIMGUI_LAYOUT_PROPERTY(Font, font, SimpleGUI::GetFont());

		SIVIMGUI_LAYOUT_PROPERTY(int32, frameThickness, 1);

		SIVIMGUI_LAYOUT_PROPERTY(int32, tabRound, 6);

		SIVIMGUI_LAYOUT_PROPERTY(Padding, tabPadding, { 2, 10 });

		SIVIMGUI_LAYOUT_PROPERTY(int32, tabSpace, 1);

		SIVIMGUI_PROPERTY(ColorF, tabActiveColor, Palette::White);

		SIVIMGUI_PROPERTY(ColorF, tabInactiveColor, ColorF{ 0.9 });

		SIVIMGUI_PROPERTY(ColorF, tabDisabledColor, ColorF{ 0.76 });

		SIVIMGUI_PROPERTY(ColorF, tabMouseOverColor, ColorF{ 1 });

		SIVIMGUI_PROPERTY(ColorF, textActiveColor, Palette::Black);

		SIVIMGUI_PROPERTY(ColorF, textInactiveColor, Palette::Black);

		SIVIMGUI_PROPERTY(ColorF, textDisabledColor, Palette::Gray);

		SIVIMGUI_PROPERTY(ColorF, textMouseOverColor, Palette::Black);

		SIVIMGUI_PROPERTY(ColorF, frameColor, ColorF{ 0.67 });

		SIVIMGUI_PROPERTY(ColorF, backgroundColor, Color::Zero());

	protected:

		int32 m_selectedTabIdx = 0;

		int32 m_hoveredTabIdx = -1;

		std::vector<Rect> m_tabRectList;

		Rect m_contentRect;

		virtual MeasureResult measure() const override;

		virtual Array<Optional<Rect>> arrange(Rect rect) override;

		virtual void update(Rect rect) override;

		virtual void draw(Rect rect) const override;

		String getTabName(size_t index) const;

		Size getTabSize(size_t index) const;
	};
}
