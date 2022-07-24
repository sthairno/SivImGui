#pragma once
#include "Common.hpp"
#include "Space.hpp"

namespace GUI::Layout
{
	enum class Alignment
	{
		Start,
		Center,
		End,
	};

	struct Default
	{
		double space = 4;

		Padding padding{ 5.0 };

		Alignment axisXAlignment = Alignment::Start;

		Alignment axisYAlignment = Alignment::Start;

		Alignment lineAlignment = Alignment::Start;
	};

	struct Horizontal
	{
		double space = 4;

		Padding padding{ 5.0 };

		Alignment axisXAlignment = Alignment::Start;

		Alignment axisYAlignment = Alignment::Start;
	};

	struct Vertical
	{
		double space = 4;

		Padding padding{ 5.0 };

		Alignment axisXAlignment = Alignment::Start;

		Alignment axisYAlignment = Alignment::Start;
	};

	inline bool operator==(const Default& l, const Default& r)
	{
		return l.space == r.space &&
			l.axisXAlignment == r.axisXAlignment &&
			l.axisYAlignment == r.axisYAlignment &&
			l.lineAlignment == r.lineAlignment;
	}

	inline bool operator!=(const Default& l, const Default& r) { return !(l == r); }

	inline bool operator==(const Horizontal& l, const Horizontal& r)
	{
		return l.space == r.space &&
			l.axisXAlignment == r.axisXAlignment &&
			l.axisYAlignment == r.axisYAlignment;
	}

	inline bool operator!=(const Horizontal& l, const Horizontal& r) { return !(l == r); }

	inline bool operator==(const Vertical& l, const Vertical& r)
	{
		return l.space == r.space &&
			l.axisXAlignment == r.axisXAlignment &&
			l.axisYAlignment == r.axisYAlignment;
	}

	inline bool operator!=(const Vertical& l, const Vertical& r) { return !(l == r); }

	using Variant = std::variant<Default, Horizontal, Vertical>;
}
