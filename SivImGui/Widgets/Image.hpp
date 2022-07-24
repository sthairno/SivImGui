#pragma once
#include "../Common.hpp"
#include "../WidgetController.hpp"

namespace GUI
{
	class Image : public WidgetController
	{
		using WidgetController::WidgetController;

	public:

		template<std::invocable<> Callback>
		Image& operator()(Callback&& f) { builderPushContainer(); f(); builderPopContainer(); return *this; }

		template<std::invocable<Image&> Callback>
		Image& operator()(Callback&& f) { builderPushContainer(); f(*this); builderPopContainer(); return *this; }

	public:

		Property<Texture> texture{ {}, *this, PropertyFlag::Layout };

		Property<ColorF> color{ Palette::White, *this, PropertyFlag::Layout };
	};

	namespace Builder
	{
		GUI::Image& Image(Context& ctx, const Texture& texture);
	}
}
