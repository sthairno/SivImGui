#pragma once
#include "../Common.hpp"
#include "../WidgetController.hpp"

namespace GUI
{
	class Button : public WidgetController
	{
		using WidgetController::WidgetController;

	public:

		template<std::invocable<> Callback>
		Button& operator()(Callback&& f) { builderPushContainer(); f(); builderPopContainer(); return *this; }

		template<std::invocable<Button&> Callback>
		Button& operator()(Callback&& f) { builderPushContainer(); f(*this); builderPopContainer(); return *this; }

	public:

		Property<ColorF> backgroundColor{ ColorF{ 1.0 }, *this };

		Property<ColorF> flameColor{ ColorF{ 0.67 }, *this };

		Property<ColorF> disabledBackgroundColor{ ColorF{ 0.92 }, *this };

		Property<ColorF> disabledFlameColor{ ColorF{ 0.67 }, *this };

		Property<ColorF> disabledTextColor{ ColorF{ 0.67 }, *this };

		Property<ColorF> mouseOverBackgroundColor{ ColorF{ 0.9, 0.95, 1.0 }, *this };

		Property<ColorF> mouseOverFlameColor{ ColorF{ 0.67 }, *this};

		Property<double> frameThickness{ 1, *this, PropertyFlag::Layout };

		Property<double> roundSize{ 4.8, *this, PropertyFlag::Layout };

		Property<bool> enabled{ true, *this };

	public:

		bool clicked() const;

		bool dblClicked() const;
	};

	namespace Builder
	{
		GUI::Button& Button(Context& ctx);
	}
}
