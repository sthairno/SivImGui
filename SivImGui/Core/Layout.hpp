#pragma once
#include <Siv3D.hpp>
#include "Space.hpp"
#include "MeasureResult.hpp"

namespace SivImGui
{
	class WidgetBase;

	enum class Alignment
	{
		Start, Center, End, Stretch
	};

	struct HorizontalLayout
	{
		Padding padding{ 0 };

		int32 space = 0;

		Alignment horizontalAlignment = Alignment::Start;

		Alignment verticalAlignment = Alignment::Stretch;

		MeasureResult measure(const std::vector<WidgetBase*>& children) const;

		Array<Optional<Rect>> arrange(Rect rect, const std::vector<WidgetBase*>& children) const;
	};

	struct VerticalLayout
	{
		Padding padding{ 0 };

		int32 space = 0;

		Alignment horizontalAlignment = Alignment::Stretch;

		Alignment verticalAlignment = Alignment::Start;

		MeasureResult measure(const std::vector<WidgetBase*>& children) const;

		Array<Optional<Rect>> arrange(Rect rect, const std::vector<WidgetBase*>& children) const;
	};

	struct StackLayout
	{
		Padding padding{ 0 };

		Alignment horizontalAlignment = Alignment::Stretch;

		Alignment verticalAlignment = Alignment::Stretch;

		MeasureResult measure(const std::vector<WidgetBase*>& children) const;

		Array<Optional<Rect>> arrange(Rect rect, const std::vector<WidgetBase*>& children) const;
	};

	using Layout = std::variant<HorizontalLayout, VerticalLayout, StackLayout>;

	inline bool operator==(const HorizontalLayout& l, const HorizontalLayout& r)
	{
		return
			l.padding == r.padding &&
			l.space == r.space &&
			l.horizontalAlignment == r.horizontalAlignment &&
			l.verticalAlignment == r.verticalAlignment;
	}

	inline bool operator!=(const HorizontalLayout& l, const HorizontalLayout& r) { return !(l == r); }

	inline bool operator==(const VerticalLayout& l, const VerticalLayout& r)
	{
		return
			l.padding == r.padding &&
			l.space == r.space &&
			l.horizontalAlignment == r.horizontalAlignment &&
			l.verticalAlignment == r.verticalAlignment;
	}

	inline bool operator!=(const VerticalLayout& l, const VerticalLayout& r) { return !(l == r); }

	inline bool operator==(const StackLayout& l, const StackLayout& r)
	{
		return
			l.padding == r.padding &&
			l.horizontalAlignment == r.horizontalAlignment &&
			l.verticalAlignment == r.verticalAlignment;
	}

	inline bool operator!=(const StackLayout& l, const StackLayout& r) { return !(l == r); }
}
