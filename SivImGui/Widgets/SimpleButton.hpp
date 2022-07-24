#pragma once
#include "../Common.hpp"
#include "../WidgetController.hpp"

namespace GUI
{
	class SimpleButton : public WidgetController
	{
		using WidgetController::WidgetController;

	public:

		template<std::invocable<> Callback>
		SimpleButton& operator()(Callback&& f) { builderPushContainer(); f(); builderPopContainer(); return *this; }

		template<std::invocable<SimpleButton&> Callback>
		SimpleButton& operator()(Callback&& f) { builderPushContainer(); f(*this); builderPopContainer(); return *this; }

	public:

		Property<Optional<double>> width{ none, *this, PropertyFlag::Layout, [](const Optional<double>& v) { return !v || v >= 0.0; } };

		Property<String> label{ U"", *this, PropertyFlag::Layout };

		Property<bool> enabled{ true, *this };

	public:

		bool clicked() const;
	};

	namespace Builder
	{
		GUI::SimpleButton& SimpleButton(Context& ctx, const String& label);
	}
}
