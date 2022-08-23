#pragma once
#include <Siv3D.hpp>

namespace SivImGui
{
	struct Space
	{
		constexpr Space(int32 all)
			: bottom(all)
			, left(all)
			, right(all)
			, top(all)
		{ }

		constexpr Space(int32 tb, int32 lr)
			: bottom(tb)
			, left(lr)
			, right(lr)
			, top(tb)
		{ }

		constexpr Space(int32 t, int32 lr, int32 b)
			: bottom(b)
			, left(lr)
			, right(lr)
			, top(t)
		{ }

		constexpr Space(int32 t, int32 l, int32 r, int32 b)
			: bottom(b)
			, left(l)
			, right(r)
			, top(t)
		{ }

		int32 bottom;

		int32 left;

		int32 right;

		int32 top;
	};

	constexpr inline bool operator==(const Space& l, const Space& r)
	{
		return l.bottom == r.bottom &&
			l.left == r.left &&
			l.right == r.right &&
			l.top == r.top;
	}

	constexpr inline Space operator+(const Space& l, const Space& r)
	{
		return {
			l.bottom + r.bottom,
			l.left + r.left,
			l.right + r.right,
			l.top + r.top
		};
	}

	constexpr inline Space operator-(const Space& l, const Space& r)
	{
		return {
			l.bottom - r.bottom,
			l.left - r.left,
			l.right - r.right,
			l.top - r.top
		};
	}

	constexpr inline Rect operator+(const Rect& r, const Space& s)
	{
		Rect rect{ r.x - s.left, r.y - s.top, s.left + r.w + s.right, s.top + r.h + s.bottom };

		if (rect.w < 0)
		{
			rect.x = r.x + r.w / 2;
			rect.w = 0;
		}

		if (rect.h < 0)
		{
			rect.y = r.y + r.h / 2;
			rect.h = 0;
		}

		return rect;
	}

	constexpr inline Rect operator-(const Rect& r, const Space& s)
	{
		Rect rect{ r.x + s.left, r.y + s.top, -s.left + r.w - s.right, -s.top + r.h - s.bottom };

		if (rect.w < 0)
		{
			rect.x = r.x + r.w / 2;
			rect.w = 0;
		}

		if (rect.h < 0)
		{
			rect.y = r.y + r.h / 2;
			rect.h = 0;
		}

		return rect;
	}

	constexpr inline Point operator+(const Point& v, const Space& s)
	{
		Point vec{ s.left + v.x + s.right, s.top + v.y + s.bottom };

		if (vec.x < 0)
		{
			vec.x = 0;
		}

		if (vec.y < 0)
		{
			vec.y = 0;
		}

		return vec;
	}

	constexpr inline Point operator-(const Point& v, const Space& s)
	{
		Point vec{ -s.left + v.x - s.right, -s.top + v.y - s.bottom };

		if (vec.x < 0)
		{
			vec.x = 0;
		}

		if (vec.y < 0)
		{
			vec.y = 0;
		}

		return vec;
	}

	constexpr inline bool operator!=(const Space& l, const Space& r) { return !(l == r); }

	constexpr inline Space& operator+=(Space& l, const Space& r) { return l = l + r; }

	constexpr inline Space& operator-=(Space& l, const Space& r) { return l = l - r; }

	constexpr inline Rect& operator+=(Rect& r, const Space& s) { return r = r + s; }

	constexpr inline Rect& operator-=(Rect& r, const Space& s) { return r = r - s; }

	constexpr inline Point& operator+=(Point& v, const Space& s) { return v = v + s; }

	constexpr inline Point& operator-=(Point& v, const Space& s) { return v = v - s; }

	using Margin = Space;

	using Padding = Space;
}
