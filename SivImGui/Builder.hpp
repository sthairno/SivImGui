#pragma once
#include "Common.hpp"
#include "Layout.hpp"

namespace GUI::Builder
{
	class Context;

	void SetLayout(Context& ctx, Layout::Variant&& layout);

	void SameLine(Context& ctx, bool value = true);
};
