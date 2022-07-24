#pragma once
#include "../Common.hpp"
#include "../WidgetController.hpp"

namespace GUI
{
	class Label : public WidgetController
	{
		using WidgetController::WidgetController;

	public:

		template<std::invocable<> Callback>
		Label& operator()(Callback&& f) { builderPushContainer(); f(); builderPopContainer(); return *this; }

		template<std::invocable<Label&> Callback>
		Label& operator()(Callback&& f) { builderPushContainer(); f(*this); builderPopContainer(); return *this; }

	public:

		Property<String> text{ U"", *this, PropertyFlag::Layout };

		Property<Optional<Font>> font{ none, *this, PropertyFlag::Layout };

		Property<ColorF> textColor{ Palette::White, *this };

	};

	namespace Builder
	{
		GUI::Label& Label(Context& ctx, const String& text);
	}
}
