#pragma once
#include <Siv3D.hpp>
#include <any>
#include "../Core/UIElement.hpp"

namespace SivImGui::Reflection
{
	struct PropertyInfo
	{
		String name;

		std::type_index type;

		PropertyFlag flag;

		std::function<std::any(const UIElement&)> getter;

		std::function<void(UIElement&, std::any v)> setter;

		std::any get(const UIElement& w) const { return getter(w); }

		void set(UIElement& w, std::any v) const { return setter(w, v); }
	};
}
