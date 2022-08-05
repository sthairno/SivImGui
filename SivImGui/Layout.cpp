#include "Layout.hpp"
#include "WidgetBase.hpp"

namespace SivImGui
{
	template<uint8 i, class T>
	static inline T& Ref(Vector2D<T>& v)
	{
		return i ? v.x : v.y;
	}

	template<uint8 i, class T>
	static inline const T& Ref(const Vector2D<T>& v)
	{
		return i ? v.x : v.y;
	}

	static double calcLeadPosition(Alignment alignment, double pos, double size, double itemSize)
	{
		switch (alignment)
		{
		case Alignment::Start: return pos;
		case Alignment::Center: return pos + (size - itemSize) / 2;
		case Alignment::End: return pos + size - itemSize;
		}
		return pos;
	}

	template<uint8 primary>
	static void arrangeVH(
		Array<RectF>& result,
		const std::vector<WidgetBase*>& children,
		const RectF rect,
		const double space,
		const Alignment primaryAlignment,
		const Alignment secondaryAlignment
	)
	{
		constexpr uint8 secondary = primary ? 0 : 1;

		std::vector<bool> expand(children.size());
		int expandCount = 0;

		const double totalSpace = (Max<size_t>(children.size(), 1) - 1) * space;
		double allocatedSize = totalSpace;

		// 分配の用意

		for (auto [idx, child] : Indexed(children))
		{
			const auto& childRequest = child->measuredSize();
			auto& childSize = result[idx].size;

			Ref<primary>(childSize) = Ref<primary>(childRequest.minSize);
			Ref<secondary>(childSize) = Ref<secondary>(childRequest.expand)
				? Max(Ref<secondary>(rect.size), Ref<secondary>(childRequest.minSize))
				: Ref<secondary>(childRequest.minSize);
		}

		for (auto [idx, child] : Indexed(children))
		{
			const auto& childRequest = child->measuredSize();
			const bool e = Ref<primary>(childRequest.expand);
			expand[idx] = e;
			if (e)
			{
				expandCount++;
			}
			else
			{
				allocatedSize += Ref<primary>(childRequest.minSize);
			}
		}

		// 残りの領域を分配

		while (expandCount > 0 && allocatedSize < Ref<primary>(rect.size))
		{
			const int prevExpandCount = expandCount;
			const double sizePerWidget = (Ref<primary>(rect.size) - allocatedSize) / expandCount;

			for (auto [idx, child] : Indexed(children))
			{
				const double minSize = Ref<primary>(child->measuredSize().minSize);
				double& size = Ref<primary>(result[idx].size);

				if (not expand[idx])
				{
					continue;
				}

				if (minSize >= sizePerWidget)
				{
					size = minSize;
					allocatedSize += size;
					expand[idx] = false;
					expandCount--;
				}
				else
				{
					size = sizePerWidget;
				}
			}

			if (expandCount == prevExpandCount)
			{
				break;
			}
		}

		// 未完了のウィジェットを完了扱いにする

		for (auto [idx, child] : Indexed(children))
		{
			double& size = Ref<primary>(result[idx].size);

			if (not expand[idx])
			{
				continue;
			}

			allocatedSize += size;
			expand[idx] = false;
			expandCount--;
		}

		// 位置の確定
		Vec2 next = rect.pos;
		Ref<primary>(next) = calcLeadPosition(primaryAlignment, Ref<primary>(rect.pos), Ref<primary>(rect.size), allocatedSize);
		for (auto [idx, child] : Indexed(children))
		{
			RectF& childRect = result[idx];
			childRect.pos = next;
			Ref<secondary>(childRect.pos) = calcLeadPosition(secondaryAlignment, Ref<secondary>(rect.pos), Ref<secondary>(rect.size), Ref<secondary>(childRect.size));
			Ref<primary>(next) += Ref<primary>(childRect.size) + space;
		}
	}

	MeasureResult HorizontalLayout::measure(const std::vector<WidgetBase*>& children) const
	{
		MeasureResult result;
		for (auto child : children)
		{
			const auto& childResult = child->measuredSize();
			result.expand.x |= childResult.expand.x;
			result.expand.y |= childResult.expand.y;
			result.minSize.x += childResult.minSize.x;
			result.minSize.y = Max(result.minSize.y, childResult.minSize.y);
		}
		result.minSize.x += (Max<size_t>(children.size(), 1) - 1) * space;
		result.minSize += padding;
		return result;
	}

	Array<RectF> HorizontalLayout::arrange(RectF rect, const std::vector<WidgetBase*>& children) const
	{
		rect -= padding;
		Array<RectF> result(children.size(), RectF{ 0, 0, 0, 0 });
		arrangeVH<1>(result, children, rect, space, axisXAlignment, axisYAlignment);
		return result;
	}

	MeasureResult VerticalLayout::measure(const std::vector<WidgetBase*>& children) const
	{
		MeasureResult result;
		for (auto child : children)
		{
			const auto& childResult = child->measuredSize();
			result.expand.x |= childResult.expand.x;
			result.expand.y |= childResult.expand.y;
			result.minSize.x = Max(result.minSize.x, childResult.minSize.x);
			result.minSize.y += childResult.minSize.y;
		}
		result.minSize.y += (Max<size_t>(children.size(), 1) - 1) * space;
		result.minSize += padding;
		return result;
	}

	Array<RectF> VerticalLayout::arrange(RectF rect, const std::vector<WidgetBase*>& children) const
	{
		rect -= padding;
		Array<RectF> result(children.size(), RectF{ 0, 0, 0, 0 });
		arrangeVH<0>(result, children, rect, space, axisYAlignment, axisXAlignment);
		return result;
	}

	MeasureResult StackLayout::measure(const std::vector<WidgetBase*>& children) const
	{
		MeasureResult result;
		for (auto child : children)
		{
			const auto& childResult = child->measuredSize();
			result.expand.x |= childResult.expand.x;
			result.expand.y |= childResult.expand.y;
			result.minSize.x = Max(result.minSize.x, childResult.minSize.x);
			result.minSize.y = Max(result.minSize.y, childResult.minSize.y);;
		}
		result.minSize += padding;
		return result;
	}

	Array<RectF> StackLayout::arrange(RectF rect, const std::vector<WidgetBase*>& children) const
	{
		rect -= padding;
		Array<RectF> result(Arg::reserve = children.size());
		for (auto child : children)
		{
			const auto& childResult = child->measuredSize();
			result.emplace_back(RectF{
				rect.pos,
				childResult.expand.x ? Max(childResult.minSize.x, rect.w) : childResult.minSize.x,
				childResult.expand.y ? Max(childResult.minSize.y, rect.h) : childResult.minSize.y,
			});
		}
		return result;
	}
}
