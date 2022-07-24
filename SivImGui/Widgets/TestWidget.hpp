#pragma once
#include "../Common.hpp"
#include "../WidgetController.hpp"

namespace GUI
{
	class TestWidget : public WidgetController
	{
		using WidgetController::WidgetController;

	public:

		template<std::invocable<> Callback>
		TestWidget& operator()(Callback&& f) { builderPushContainer(); f(); builderPopContainer(); return *this; }

		template<std::invocable<TestWidget&> Callback>
		TestWidget& operator()(Callback&& f) { builderPushContainer(); f(*this); builderPopContainer(); return *this; }

	public:

		Property<String> text{ U"test", *this, PropertyFlag::Layout };

		Property<double> frameThickness{ 1.0, *this, PropertyFlag::None, [](double v) { return v >= 0.0; } };

		Property<ColorF> frameColor{ Palette::White, *this };
	};

	namespace Builder
	{
		GUI::TestWidget& TestWidget(Context& ctx);
	}
}
