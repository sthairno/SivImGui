#pragma once
#include <Siv3D.hpp>
#include "Space.hpp"
#include "MeasureResult.hpp"

namespace SivImGui
{
	class WidgetBase;

	enum class Alignment
	{
		Start, Center, End
	};

	struct HorizontalLayout
	{
		Padding padding{ 10.0 };

		double space = 5.0;

		Alignment axisXAlignment = Alignment::Start;

		Alignment axisYAlignment = Alignment::Start;

		MeasureResult measure(const std::vector<WidgetBase*>& children) const;

		Array<RectF> arrange(RectF rect, const std::vector<WidgetBase*>& children) const;
	};

	struct VerticalLayout
	{
		Padding padding{ 10.0 };

		double space = 5.0;

		Alignment axisXAlignment = Alignment::Start;

		Alignment axisYAlignment = Alignment::Start;

		MeasureResult measure(const std::vector<WidgetBase*>& children) const;

		Array<RectF> arrange(RectF rect, const std::vector<WidgetBase*>& children) const;
	};

	struct StackLayout
	{
		Padding padding{ 10.0 };

		MeasureResult measure(const std::vector<WidgetBase*>& children) const;

		Array<RectF> arrange(RectF rect, const std::vector<WidgetBase*>& children) const;
	};

	struct Layout
	{
		using Variant = std::variant<HorizontalLayout, VerticalLayout, StackLayout>;

		Layout(Variant data)
			: data(data)
		{}

		Variant data;

		MeasureResult measure(const std::vector<WidgetBase*>& children) const
		{
			return std::visit([&](auto& l) { return l.measure(children); }, data);
		}

		Array<RectF> arrange(RectF rect, const std::vector<WidgetBase*>& children) const
		{
			return std::visit([&](auto& l) { return l.arrange(rect, children); }, data);
		}
	};

	inline bool operator==(const HorizontalLayout& l, const HorizontalLayout& r)
	{
		return
			l.padding == r.padding &&
			l.space == r.space &&
			l.axisXAlignment == r.axisXAlignment &&
			l.axisYAlignment == r.axisYAlignment;
	}

	inline bool operator!=(const HorizontalLayout& l, const HorizontalLayout& r) { return !(l == r); }

	inline bool operator==(const VerticalLayout& l, const VerticalLayout& r)
	{
		return
			l.padding == r.padding &&
			l.space == r.space &&
			l.axisXAlignment == r.axisXAlignment &&
			l.axisYAlignment == r.axisYAlignment;
	}

	inline bool operator!=(const VerticalLayout& l, const VerticalLayout& r) { return !(l == r); }

	inline bool operator==(const StackLayout& l, const StackLayout& r)
	{
		return
			l.padding == r.padding;
	}

	inline bool operator!=(const StackLayout& l, const StackLayout& r) { return !(l == r); }

	inline bool operator==(const Layout& l, const Layout& r) { return l.data == r.data; }

	inline bool operator!=(const Layout& l, const Layout& r) { return l.data != r.data; }
}
