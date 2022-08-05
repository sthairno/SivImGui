#pragma once
#include <Siv3D.hpp>

namespace SivImGui
{
	struct MeasureResult
	{
		SizeF minSize = { 0, 0 };

		Vector2D<bool> expand = { false, false };
	};
}
