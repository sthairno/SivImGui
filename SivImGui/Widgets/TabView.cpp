#include "TabView.hpp"

namespace SivImGui
{
	static std::pair<Polygon, Polygon> BuildActiveTabShape(Rect contentRect, Rect tabRect, int32 tabRound, int32 frameThickness)
	{
		Array<Vec2> outerVertices =
			tabRect
			.rounded(tabRound, tabRound, 0, 0)
			.outer();
		outerVertices.insert(outerVertices.cend() - 1, { contentRect.tr(), contentRect.br(), contentRect.bl(), contentRect.tl() });
		outerVertices.unique_consecutive();

		Array<Vec2> innerVertices;
		if (tabRound <= frameThickness)
		{
			innerVertices = tabRect
				.stretched(-frameThickness, -frameThickness, frameThickness, -frameThickness)
				.outline();
		}
		else
		{
			int32 r = tabRound - frameThickness;
			innerVertices =
				tabRect
				.stretched(-frameThickness, -frameThickness, frameThickness, -frameThickness)
				.rounded(r, r, 0, 0)
				.outer();
		}
		innerVertices.insert(
			innerVertices.cend() - 1,
			{
				contentRect.tr() + Point{ -frameThickness, frameThickness },
				contentRect.br() + Point{ -frameThickness, -frameThickness },
				contentRect.bl() + Point{ frameThickness, -frameThickness },
				contentRect.tl() + Point{ frameThickness, frameThickness },
			});
		innerVertices.unique_consecutive();

		auto result = static_cast<uint8>(Polygon::Validate(outerVertices, { innerVertices.reversed() }));
		assert(result == 0);
		return {
			Polygon{ outerVertices, { innerVertices.reversed() } },
			Polygon{ innerVertices },
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

		MeasureResult result = std::visit([this](auto& l) {
			WidgetBase* selectedItem = visibleChildren()[m_selectedTabIdx];
			return l.measure({ selectedItem });
		}, layout.value());
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

		result.minSize.x = Min(result.minSize.x, tabAreaSize.x);
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

		for (auto [idx, child] : Indexed(visibleChildren()))
		{
			const Rect tabRect = m_tabRectList[idx];

			if (idx == m_selectedTabIdx)
			{
				auto [frame, content] =
					BuildActiveTabShape(m_contentRect, tabRect, tabRound, frameThickness);
				
				frame.draw(frameColor);
				content.draw(backgroundColor);
				drawChildren({ child });
			}
			else
			{
				auto [frame, content] =
					BuildInactiveTabShape(tabRect, tabRound, frameThickness);

				frame.draw(frameColor);
				content.draw(tabInactiveColor);
			}

			(*font)(getTabName(idx))
				.drawAt((tabRect - Padding{ frameThickness, frameThickness, 0 }).center(), Palette::Black);
		}
	}

	String TabView::getTabName(size_t index) const
	{
		StringView name = visibleChildren()[index]->name.value();
		if (name.empty())
		{
			return U"Tab {}"_fmt(index);
		}
		return String{ name };
	}

	Size TabView::getTabSize(size_t index) const
	{
		Size size =
			(*font)(getTabName(index)).region().size.asPoint()
			+ tabPadding
			+ Padding{ frameThickness, frameThickness, 0 };
		return { Max<int32>(size.x * 2, tabRound), Max<int32>(size.y, tabRound) };
	}
}
