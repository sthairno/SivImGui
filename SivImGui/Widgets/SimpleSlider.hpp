#pragma once
#include "../Common.hpp"
#include "../WidgetController.hpp"

namespace GUI
{
	class SimpleSlider : public WidgetController
	{
		using WidgetController::WidgetController;

	public:

		template<std::invocable<> Callback>
		SimpleSlider& operator()(Callback&& f) { builderPushContainer(); f(); builderPopContainer(); return *this; }

		template<std::invocable<SimpleSlider&> Callback>
		SimpleSlider& operator()(Callback&& f) { builderPushContainer(); f(*this); builderPopContainer(); return *this; }

	public:

		Property<double*> ref{ nullptr, *this, PropertyFlag::Layout };

		Property<double> min{ 0.0, *this, PropertyFlag::Layout };

		Property<double> max{ 1.0, *this, PropertyFlag::Layout };

		Property<double> width{ 120.0, *this, PropertyFlag::Layout, [](const Optional<double>& v) { return !v || v >= 0.0; } };

		Property<bool> enabled{ true, *this };

	public:

		bool valueChanged() const;
	};

	namespace Builder
	{
		GUI::SimpleSlider& SimpleSlider(Context& ctx, double& value);
	}
}
