#pragma once
#include <Siv3D.hpp>

namespace SivImGui
{
	struct WidgetTypeInfo
	{
		size_t id;

		StringView name;

		bool enableMouseOver = true;
	};
}
