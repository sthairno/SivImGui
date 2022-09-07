#pragma once
#include <Siv3D.hpp>
#include <any>
#include "../WidgetBase.hpp"

namespace SivImGui::Reflection
{
	struct PropertyInfo
	{
		String name;

		std::type_index type;

		PropertyFlag flag;

		std::function<std::any(const WidgetBase&)> getter;

		std::function<void(WidgetBase&, std::any v)> setter;

		std::any get(const WidgetBase& w) const { return getter(w); }

		void set(WidgetBase& w, std::any v) const { return setter(w, v); }
	};
}
