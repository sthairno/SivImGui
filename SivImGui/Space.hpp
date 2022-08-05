#pragma once
#include <Siv3D.hpp>

namespace SivImGui
{
	struct Space
	{
		constexpr Space(double all)
			: bottom(all)
			, left(all)
			, right(all)
			, top(all)
		{ }

		constexpr Space(double tb, double lr)
			: bottom(tb)
			, left(lr)
			, right(lr)
			, top(tb)
		{ }

		constexpr Space(double t, double lr, double b)
			: bottom(b)
			, left(lr)
			, right(lr)
			, top(t)
		{ }

		constexpr Space(double t, double l, double r, double b)
			: bottom(b)
			, left(l)
			, right(r)
			, top(t)
		{ }

		double bottom;

		double left;

		double right;

		double top;
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

	constexpr inline RectF operator+(const RectF& r, const Space& s)
	{
		RectF rect{ r.x - s.left, r.y - s.top, s.left + r.w + s.right, s.top + r.h + s.bottom };

		if (rect.w < 0.0)
		{
			rect.x = r.x + r.w / 2;
			rect.w = 0.0;
		}

		if (rect.h < 0.0)
		{
			rect.y = r.y + r.h / 2;
			rect.h = 0.0;
		}

		return rect;
	}

	constexpr inline RectF operator-(const RectF& r, const Space& s)
	{
		RectF rect{ r.x + s.left, r.y + s.top, -s.left + r.w - s.right, -s.top + r.h - s.bottom };

		if (rect.w < 0.0)
		{
			rect.x = r.x + r.w / 2;
			rect.w = 0.0;
		}

		if (rect.h < 0.0)
		{
			rect.y = r.y + r.h / 2;
			rect.h = 0.0;
		}

		return rect;
	}

	constexpr inline Vec2 operator+(const Vec2& v, const Space& s)
	{
		Vec2 vec{ s.left + v.x + s.right, s.top + v.y + s.bottom };

		if (vec.x < 0.0)
		{
			vec.x = 0.0;
		}

		if (vec.y < 0.0)
		{
			vec.y = 0.0;
		}

		return vec;
	}

	constexpr inline Vec2 operator-(const Vec2& v, const Space& s)
	{
		Vec2 vec{ -s.left + v.x - s.right, -s.top + v.y - s.bottom };

		if (vec.x < 0.0)
		{
			vec.x = 0;
		}

		if (vec.y < 0.0)
		{
			vec.y = 0.0;
		}

		return vec;
	}

	constexpr inline bool operator!=(const Space& l, const Space& r) { return !(l == r); }

	constexpr inline Space& operator+=(Space& l, const Space& r) { return l = l + r; }

	constexpr inline Space& operator-=(Space& l, const Space& r) { return l = l - r; }

	constexpr inline RectF& operator+=(RectF& r, const Space& s) { return r = r + s; }

	constexpr inline RectF& operator-=(RectF& r, const Space& s) { return r = r - s; }

	constexpr inline Vec2& operator+=(Vec2& v, const Space& s) { return v = v + s; }

	constexpr inline Vec2& operator-=(Vec2& v, const Space& s) { return v = v - s; }

	using Margin = Space;

	using Padding = Space;
}
