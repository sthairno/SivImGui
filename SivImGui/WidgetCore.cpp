#include "WidgetCore.hpp"
#include "Manager.hpp"
#include "BuilderContext.hpp"
#include "WidgetController.hpp"

namespace GUI
{
	static double calcLeadPosition(Layout::Alignment alignment, double pos, double size, double itemSize)
	{
		using Layout::Alignment;
		switch (alignment)
		{
		case Alignment::Start: return pos;
		case Alignment::Center: return pos + (size - itemSize) / 2;
		case Alignment::End: return pos + size - itemSize;
		}
		return pos;
	}

	Vec2 WidgetCore::globalPos() noexcept
	{
		if (not m_parent)
		{
			return pos();
		}
		return m_parent->globalPos() + pos() - m_parent->m_scrollPos;
	}

	void WidgetCore::makeDirty() noexcept
	{
		//Console << fmt::format(U"{:>{}}-->{}", U"", m_depth * 3, U"MakeDirty");

		if (m_parent)
		{
			m_parent->makeDirty();
		}
		m_dirtyLayout = true;
	}

	void WidgetCore::makeAllDirty() noexcept
	{
		for (auto& child : children())
		{
			child->makeAllDirty();
		}
		makeDirty();
	}

	void WidgetCore::updateLayout(Optional<SizeF> availableSize)
	{
		if (availableSize)
		{
			m_availableSize = *availableSize;
		}
		auto prevResult = m_measureResult;
		auto newResult = measure(*m_availableSize);
		if (prevResult != newResult && m_parent)
		{
			m_parent->updateLayout();
		}
		else
		{
			arrange(m_rect.pos);
		}
	}

	void WidgetCore::draw()
	{
		m_rect.drawFrame(1, Palette::Red);
		Transformer2D t(
			Mat3x2::Translate(m_rect.pos - (m_parent ? m_parent->m_scrollPos : Vec2{ 0, 0 })),
			TransformCursor::Yes);
		drawImpl({ 0, 0, m_rect.size });
	}

	MeasureResult WidgetCore::measure(SizeF availableSize)
	{
		if (not m_dirtyLayout && m_availableSize == availableSize)
		{
			return *m_measureResult;
		}

		Console << fmt::format(U"{:>{}}-->", U"", m_depth * 3) << availableSize;

		m_availableSize = availableSize;

		auto result = measureImpl(availableSize);
		assert(MeasureResult::Validate(result, availableSize));

		Console << fmt::format(U"{:>{}}<--", U"", m_depth * 3) << result.desiredSize;

		m_measureResult = result;
		m_rect.size = result.desiredSize;
		return result;
	}

	void WidgetCore::arrange(Vec2 pos)
	{
		m_rect.pos = pos;

		if (not m_dirtyLayout)
		{
			return;
		}

		Console << fmt::format(U"{:>{}}", U"", m_depth * 3) << pos;

		arrangeImpl({ 0, 0, m_rect.size });
		m_dirtyLayout = false;
	}

	void WidgetCore::procPositionedEvent(Event& e, Vec2 offset)
	{
		const RectF globalRect{ offset + pos(), size() };

		assert(e.pos);
		assert(globalRect == this->globalRect());

		if (not globalRect.contains(*e.pos))
		{
			return;
		}

		for (auto& child : children())
		{
			child->procPositionedEvent(e, globalRect.pos - m_scrollPos);
			if (e.handled)
			{
				return;
			}
		}
		onEvent(e, globalRect);
	}

	MeasureResult WidgetCore::measureContainer(SizeF availableSize)
	{
		const auto l = getController().layout.value();
		const auto [space, padding] =
			std::visit([](auto& v) { return std::pair{ v.space, v.padding }; }, l);
		availableSize -= padding;

		MeasureResult result;

		switch (l.index())
		{
		case 0: // Default
		{
			const auto& layout = std::get<Layout::Default>(l);

			SizeF desiredSize{ 0, 0 };
			Size childCount{ 0, 1 };

			{
				int columnCount = 0;
				SizeF lineSize{ 0, 0 };
				for (auto [idx, child] : IndexedRef(children()))
				{
					const auto childResult = child->measure({ 0, 0 });
					const bool sameLine = child->getController().sameLine;

					columnCount++;
					childCount.x = Max(childCount.x, columnCount);

					lineSize.x += childResult.desiredSize.x;
					lineSize.y = Max(lineSize.y, childResult.desiredSize.y);

					if (not sameLine && idx != children().size() - 1)
					{
						desiredSize.x = Max(desiredSize.x, lineSize.x);
						desiredSize.y += lineSize.y;

						columnCount = 0;
						childCount.y++;
						lineSize = { 0, 0 };
					}
				}

				desiredSize.x = Max(desiredSize.x, lineSize.x);
				desiredSize.y += lineSize.y;
			}

			result.desiredSize = desiredSize + SizeF{
				Max(childCount.x - 1, 0) * space,
				Max(childCount.y - 1, 0) * space
			};
		}
		break;
		case 1: // Horizontal
		case 2: // Vertical
		{
			std::vector<bool> isTight(children().size(), true);
			int tightChildCount = children().size();

			const double totalSpace = Max<int>(children().size() - 1, 0) * space;
			const double availableInnerSize = Max(0.0, availableSize.x - totalSpace);
			double availableSizePerChild = 0.0;

			// 最小幅(0.0)でサイズ測定→余った領域を分配して再測定
			while(true)
			{
				double allocatedSize = 0.0;

				if (l.index() == 1)
				{
					for (auto [idx, child] : IndexedRef(children()))
					{
						if (isTight[idx])
						{
							auto childResult = child->measure({ availableSizePerChild, availableSize.y });
							if (not childResult.isTight.x)
							{
								isTight[idx] = false;
								tightChildCount--;
							}
						}
						allocatedSize += child->measureResult()->desiredSize.x;
					}
				}
				else // if (l.index() == 2)
				{
					for (auto [idx, child] : IndexedRef(children()))
					{
						if (isTight[idx])
						{
							auto childResult = child->measure({ availableSize.x, availableSizePerChild });
							if (not childResult.isTight.y)
							{
								isTight[idx] = false;
								tightChildCount--;
							}
						}
						allocatedSize += child->measureResult()->desiredSize.y;
					}
				}

				if (tightChildCount == 0 ||
					allocatedSize >= availableInnerSize)
				{
					break;
				}

				double newSize = (availableInnerSize - allocatedSize) / tightChildCount;
				assert(newSize != availableSizePerChild);
				availableSizePerChild = newSize;
			}

			// サイズを計算
			if (l.index() == 1)
			{
				for (const auto& child : children())
				{
					auto childResult = child->measureResult().value();

					result.desiredSize.x += childResult.desiredSize.x;
					result.isTight.x |= childResult.isTight.x;
					if (childResult.desiredSize.y >= result.desiredSize.y)
					{
						result.desiredSize.y = childResult.desiredSize.y;
						result.isTight.y |= childResult.isTight.y;
					}
				}
				result.desiredSize.x += totalSpace;
			}
			else // if (l.index() == 2)
			{
				for (const auto& child : children())
				{
					auto childResult = child->measureResult().value();

					if (childResult.desiredSize.x >= result.desiredSize.x)
					{
						result.desiredSize.x = childResult.desiredSize.x;
						result.isTight.x |= childResult.isTight.x;
					}
					result.desiredSize.y += childResult.desiredSize.y;
					result.isTight.y |= childResult.isTight.y;
				}
				result.desiredSize.y += totalSpace;
			}
		}
		break;
		}

		result.desiredSize += padding;
		return result;
	}

	void WidgetCore::arrangeContainer(RectF rect)
	{
		const auto l = getController().layout.value();
		const Padding padding =
			std::visit([](auto& v) { return v.padding; }, l);
		rect -= padding;

		switch (l.index())
		{
		case 0: // Default
		{
			const auto& layout = std::get<Layout::Default>(l);

			std::vector<std::pair<SizeF, std::vector<WidgetCore*>>> lineList;

			// +------------------------+
			// |+----------------------+|
			// ||         Line         ||
			// ||+---+            +---+||
			// |||   | ---------> |   |||
			// ||+---+            |   |||
			// || (SameLine:true) +---+||
			// |+----------------------+|
			// |                    |   |
			// |  +-----------------+   |
			// |  | (SameLine:false)    |
			// |  V                     |
			// |+----------+            |
			// ||   Line   |            |
			// |+----------+            |
			// +------------------------+

			// generate lineList
			lineList.emplace_back(std::pair{ SizeF{ 0, 0 }, std::vector<WidgetCore*>{} });
			for (auto [idx, child] : IndexedRef(children()))
			{
				auto& [lineSize, lineWidgets] = lineList.back();
				const bool sameLine = child->getController().sameLine;
				const SizeF childSize = child->size();

				lineSize.y = Max(lineSize.y, childSize.y);
				lineSize.x += childSize.x;
				lineWidgets.push_back(child.get());

				if (not sameLine && idx != children().size() - 1)
				{
					lineList.emplace_back(std::pair{ SizeF{ 0, 0 }, std::vector<WidgetCore*>{} });
				}
			}

			double totalHeight = Max<size_t>(lineList.size() - 1, 0) * layout.space;
			for (auto& [lineSize, widgets] : lineList)
			{
				lineSize.x += Max<size_t>(widgets.size() - 1, 0) * layout.space;
				totalHeight += lineSize.y;
			}

			// arrange
			double nextLineY = calcLeadPosition(
				layout.axisYAlignment,
				rect.y, rect.h, totalHeight
			);
			for (auto& [lineSize, widgets] : lineList)
			{
				const Vec2 linePos {
					calcLeadPosition(
						layout.axisXAlignment,
						rect.x, rect.w, lineSize.x
					),
					nextLineY
				};

				double nextWidgetX = linePos.x;
				for (auto widget : widgets)
				{
					const SizeF widgetSize = widget->size();
					const Vec2 widgetPos = {
						nextWidgetX,
						calcLeadPosition(
							layout.lineAlignment,
							linePos.y, lineSize.y, widgetSize.y
						)
					};

					widget->arrange(widgetPos);

					nextWidgetX += widget->size().x + layout.space;
				}

				nextLineY += lineSize.y + layout.space;
			}

			return;
		}
		case 1: // Horizontal
		{
			const auto& layout = std::get<Layout::Horizontal>(l);
			const double totalWidth = std::accumulate(
				children().cbegin(),
				children().cend(),
				Max<size_t>(children().size() - 1, 0) * layout.space,
				[](double acc, const std::unique_ptr<WidgetCore>& v) {
				return acc + v->size().x;
			});

			double nextChildX = calcLeadPosition(
				layout.axisXAlignment,
				rect.x, rect.w, totalWidth
			);
			for (auto& child : children())
			{
				const SizeF childSize = child->size();
				const Vec2 childPos{
					nextChildX,
					calcLeadPosition(
						layout.axisYAlignment,
						rect.y, rect.h, childSize.y
					)
				};

				child->arrange(childPos);

				nextChildX += childSize.x + layout.space;
			}
			return;
		}
		case 2: // Vertical
		{
			const auto& layout = std::get<Layout::Vertical>(l);
			const double totalHeight = std::accumulate(
				children().cbegin(),
				children().cend(),
				Max<size_t>(children().size() - 1, 0) * layout.space,
				[](double acc, const std::unique_ptr<WidgetCore>& v) {
				return acc + v->size().y;
			});

			double nextChildY = calcLeadPosition(
				layout.axisYAlignment,
				rect.y, rect.h, totalHeight
			);
			for (auto& child : children())
			{
				const SizeF childSize = child->size();
				const Vec2 childPos{
					calcLeadPosition(
						layout.axisXAlignment,
						rect.x, rect.w, childSize.x
					),
					nextChildY
				};

				child->arrange(childPos);

				nextChildY += childSize.y + layout.space;
			}
			return;
		}
		}
		throw Error(U"Not implimented");
	}

	MeasureResult WidgetCore::measureImpl(SizeF availableSize)
	{
		return isContainer()
			? measureContainer(availableSize)
			: MeasureResult{ };
	}

	void WidgetCore::arrangeImpl(RectF rect)
	{
		if (isContainer())
		{
			arrangeContainer(rect);
		}
	}

	void WidgetCore::drawImpl(RectF)
	{
		for (auto& child : children())
		{
			child->draw();
		}
	}

	void WidgetCore::onEvent(Event&, RectF)
	{ }
}
