#include "TabView.hpp"

namespace SivImGui
{
	static std::tuple<Polygon, Polygon, Rect> BuildActiveTabShape(Rect contentRect, Rect tabRect, int32 tabRound, int32 frameThickness)
	{
		Polygon innerTabPolygon;
		if (tabRound <= frameThickness)
		{
			innerTabPolygon =
				tabRect
				.stretched(-frameThickness, -frameThickness, frameThickness, -frameThickness)
				.asPolygon();
		}
		else
		{
			int32 r = tabRound - frameThickness;
			innerTabPolygon =
				tabRect
				.stretched(-frameThickness, -frameThickness, frameThickness, -frameThickness)
				.rounded(r, r, 0, 0);
		}

		Rect innerContentRect = contentRect.stretched(-frameThickness);

		if (frameThickness <= 0)
		{
			return {
				Polygon{ },
				innerTabPolygon,
				innerContentRect
			};
		}

		Array<Vec2> outerVertices =
			tabRect
			.rounded(tabRound, tabRound, 0, 0)
			.outer();
		outerVertices.insert(outerVertices.cend() - 1, { contentRect.tr(), contentRect.br(), contentRect.bl(), contentRect.tl() });
		outerVertices.unique_consecutive();

		Array<Vec2> reversedInnerVertices(Arg::reserve = innerTabPolygon.vertices().size() + 4);
		reversedInnerVertices.insert(reversedInnerVertices.end(), innerTabPolygon.vertices().back());
		reversedInnerVertices.insert(reversedInnerVertices.end(), { innerContentRect.tl(), innerContentRect.bl(), innerContentRect.br(), innerContentRect.tr() });
		reversedInnerVertices.insert(reversedInnerVertices.end(), innerTabPolygon.vertices().crbegin() + 1, innerTabPolygon.vertices().crend());
		reversedInnerVertices.unique_consecutive();

		return {
			Polygon{ outerVertices, { reversedInnerVertices } },
			innerTabPolygon,
			innerContentRect
		};
	}

	static std::pair<Polygon, Polygon> BuildInactiveTabShape(Rect tabRect, int32 tabRound, int32 frameThickness)
	{
		const Array<Vec2> outerVertices =
			tabRect
			.rounded(tabRound, tabRound, 0, 0)
			.outer();

		if (frameThickness <= 0)
		{
			return {
				Polygon{ },
				Polygon{ outerVertices },
			};
		}

		Array<Vec2> innerVertices;
		if (tabRound <= frameThickness)
		{
			innerVertices = tabRect
				.stretched(-frameThickness, -frameThickness, 0, -frameThickness)
				.outline();
		}
		else
		{
			int32 r = tabRound - frameThickness;
			innerVertices =
				tabRect
				.stretched(-frameThickness, -frameThickness, 0, -frameThickness)
				.rounded(r, r, 0, 0)
				.outer();
		}

		Array<Vec2> frameVertices(Arg::reserve = outerVertices.size() + innerVertices.size());
		frameVertices.insert(frameVertices.end(), outerVertices.back());
		frameVertices.insert(frameVertices.end(), outerVertices.begin(), outerVertices.end() - 1);
		frameVertices.insert(frameVertices.end(), innerVertices.rbegin() + 1, innerVertices.rend());
		frameVertices.insert(frameVertices.end(), innerVertices.back());

		return {
			Polygon{ frameVertices },
			Polygon{ innerVertices },
		};
	}

	MeasureResult TabView::measure() const
	{
		if (visibleChildren().empty())
		{
			return {
				.minSize = { frameThickness * 2, frameThickness * 2 }
			};
		}

		for (WidgetBase* child : visibleChildren())
		{
			std::visit([child](auto& l) {
				return l.measure({ child });
			}, layout.value());
		}

		MeasureResult result = visibleChildren()[m_selectedTabIdx]->measuredSize();
		result.minSize += Size{
			frameThickness * 2,
			frameThickness * 2
		};

		Size tabAreaSize{ 0, 0 };
		for (size_t idx : Iota(visibleChildren().size()))
		{
			const Size tabSize = getTabSize(idx);
			tabAreaSize.x += tabSize.x;
			tabAreaSize.y = Max(tabAreaSize.y, tabSize.y);
		}
		tabAreaSize.x += (static_cast<int32>(visibleChildren().size()) - 1) * tabSpace;
		result.minSize.x = Max(result.minSize.x, tabAreaSize.x);
		result.minSize.y += tabAreaSize.y;

		return result;
	}

	Array<Optional<Rect>> TabView::arrange(Rect rect)
	{
		if (visibleChildren().empty())
		{
			m_tabRectList = { };
			m_contentRect = rect;
			return { };
		}

		Point nextTabPos{ 0, 0 };
		m_tabRectList.clear();
		m_tabRectList.reserve(visibleChildren().size());

		int32 tabHeight = 0;
		for (size_t idx : Iota(visibleChildren().size()))
		{
			const Size tabSize = getTabSize(idx);
			tabHeight = Max(tabHeight, tabSize.y);

			m_tabRectList.emplace_back(Rect{
				nextTabPos,
				tabSize
			});
			nextTabPos.x += tabSize.x;
			nextTabPos.x += tabSpace;
		}
		for (auto& tabRect : m_tabRectList)
		{
			tabRect.h = tabHeight;
		}
		m_contentRect = rect.stretched(-tabHeight, 0, 0, 0);

		Array<Optional<Rect>> result(visibleChildren().size(), none);
		result[m_selectedTabIdx] = std::visit([this](auto& l) {
			WidgetBase* selectedItem = visibleChildren()[m_selectedTabIdx];
			return l.arrange(m_contentRect - Padding{ frameThickness }, { selectedItem });
		}, layout.value())[0].value();
		return result;
	}

	void TabView::update(Rect rect)
	{
		if (visibleChildren().empty())
		{
			return;
		}

		m_hoveredTabIdx = -1;

		if (mouseOver())
		{
			for (auto [idx, child] : Indexed(visibleChildren()))
			{
				const bool isActive = idx == m_selectedTabIdx;
				const bool isEnabled = this->isEnabled() && child->enabled;

				if (isActive || not isEnabled)
				{
					continue;
				}

				const Rect tabRect = m_tabRectList[idx];

				if (tabRect.mouseOver())
				{
					m_hoveredTabIdx = idx;
				}
			}
		}

		if (m_hoveredTabIdx != -1 &&
			MouseL.down())
		{
			m_selectedTabIdx = m_hoveredTabIdx;
			requestLayout();
		}

		WidgetBase* selectedItem = visibleChildren()[m_selectedTabIdx];
		updateChildren({ selectedItem });
	}


	void TabView::draw(Rect rect) const
	{
		if (visibleChildren().empty())
		{
			rect.drawFrame(frameThickness, 0, frameColor);
			return;
		}

		if (m_hoveredTabIdx != -1)
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		for (auto [idx, child] : Indexed(visibleChildren()))
		{
			const bool isActive = idx == m_selectedTabIdx;
			const bool isHovered = idx == m_hoveredTabIdx;
			const bool isEnabled = this->isEnabled() && child->enabled;
			const Rect tabRect = m_tabRectList[idx];

			ColorF tabColor, textColor;
			if (isActive)
			{
				tabColor = tabActiveColor;
				textColor = textActiveColor;
			}
			else if (not isEnabled)
			{
				tabColor = tabDisabledColor;
				textColor = textDisabledColor;
			}
			else if (isHovered)
			{
				tabColor = tabMouseOverColor;
				textColor = textMouseOverColor;
			}
			else
			{
				tabColor = tabInactiveColor;
				textColor = textInactiveColor;
			}

			if (isActive)
			{
				auto [frame, tab, content] =
					BuildActiveTabShape(m_contentRect, tabRect, tabRound, frameThickness);
				
				frame.draw(frameColor);
				tab.draw(tabColor);

				content.draw(backgroundColor);
				drawChildren({ child });
			}
			else
			{
				auto [frame, tab] =
					BuildInactiveTabShape(tabRect, tabRound, frameThickness);

				frame.draw(frameColor);
				tab.draw(tabColor);
			}

			(*font)(getTabName(idx))
				.drawAt((tabRect - Padding{ frameThickness, frameThickness, 0 }).center(), textColor);
		}
	}

	String TabView::getTabName(size_t index) const
	{
		StringView name = visibleChildren()[index]->name.value();
		if (name.empty())
		{
			return U"Tab {}"_fmt(index + 1);
		}
		return String{ name };
	}

	Size TabView::getTabSize(size_t index) const
	{
		Size size =
			(*font)(getTabName(index)).region().size.asPoint()
			+ tabPadding
			+ Padding{ frameThickness, frameThickness, 0 };
		return { Max<int32>(size.x, tabRound * 2), Max<int32>(size.y, tabRound) };
	}
}
