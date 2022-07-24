#include <Siv3D.hpp> // OpenSiv3D v0.6.4
#include "Manager.hpp"
#include "EventProvider.hpp"
#include "Widgets/TestWidget.hpp"
#include "Widgets/SimpleButton.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/SimpleSlider.hpp"

namespace GUI::Builder
{
	GUI::Button& ImageButton(Context& ctx, const Texture& texture, const String& str)
	{
		return GUI::Builder::Button(ctx)([&] {
			GUI::Builder::SetLayout(ctx, GUI::Layout::Horizontal{
				.padding = { 2, 10 },
				.axisYAlignment = GUI::Layout::Alignment::Center,
			});
			GUI::Builder::Image(ctx, texture).color = Palette::Black;
			GUI::Builder::Label(ctx, str).textColor = Palette::Black;
		});
	}

	GUI::Button& TextButton(Context& ctx, const String& str)
	{
		return GUI::Builder::Button(ctx)([&] {
			GUI::Builder::SetLayout(ctx, GUI::Layout::Horizontal{
				.padding = { 2, 10 }
			});
			GUI::Builder::Label(ctx, str).textColor = Palette::Black;
		});
	}
}

void Main()
{
	Scene::SetBackground(ColorF(0.9));

	GUI::Manager manager;
	auto& ctx = manager.getBuilder();

	const Texture icon1(0xf013_icon, 34);
	
	String dump;

	double frameThickness = 20;
	double roundSize = 10;

	while (System::Update())
	{
		const ScopedRenderStates2D rasterizer{ RasterizerState::WireframeCullNone };
		ClearPrint();
		manager.begin();

		//GUI::Builder::TestWidget(ctx)([&] {
		//	GUI::Builder::SetLayout(ctx, GUI::Layout::Vertical{
		//		.padding = { 10 },
		//	});
		//	GUI::Builder::TestWidget(ctx)([&] {
		//		GUI::Builder::SetLayout(ctx, GUI::Layout::Horizontal{
		//			.space = 0.0,
		//			.axisYAlignment = GUI::Layout::Alignment::Center
		//		});
		//		GUI::Builder::Label(ctx, U"FrameThickness").textColor = Palette::Black;
		//		GUI::Builder::SimpleSlider(ctx, frameThickness)([&](GUI::SimpleSlider& s) {
		//			s.width = 300.0;
		//			s.min = 0.0;
		//			s.max = 100.0;
		//		});
		//		GUI::Builder::Label(ctx, fmt::format(U"{:.1f}", frameThickness)).textColor = Palette::Black;
		//	});
		//	GUI::Builder::TestWidget(ctx)([&] {
		//		GUI::Builder::SetLayout(ctx, GUI::Layout::Horizontal{
		//			.space = 0.0,
		//			.axisYAlignment = GUI::Layout::Alignment::Center
		//		});
		//		GUI::Builder::Label(ctx, U"RoundSize").textColor = Palette::Black;
		//		GUI::Builder::SimpleSlider(ctx, roundSize)([&](GUI::SimpleSlider& s) {
		//			s.width = 300.0;
		//			s.min = 0.0;
		//			s.max = 100.0;
		//		});
		//		GUI::Builder::Label(ctx, fmt::format(U"{:.1f}", roundSize)).textColor = Palette::Black;
		//	});
		//});

		//GUI::Builder::TestWidget(ctx)([&] {
		//	GUI::Builder::SetLayout(ctx, GUI::Layout::Horizontal{
		//		.padding = { 10 },
		//	});

		//	if (GUI::Builder::SimpleButton(ctx, U"SimpleButton").clicked())
		//	{
		//		// do something
		//	}

		//	if (GUI::Builder::TextButton(ctx, U"TextButton")
		//		([&](GUI::Button& b){ b.frameThickness = frameThickness; b.roundSize = roundSize; })
		//		.clicked())
		//	{
		//		// do something
		//	}

		//	if (GUI::Builder::ImageButton(ctx, icon1, U"IconButton").clicked())
		//	{
		//		// do something
		//	}
		//});

		//GUI::Builder::TestWidget(ctx)([&] {
		//	GUI::Builder::Label(ctx, dump).textColor = Palette::Black;
		//});

		manager.end();
		dump = manager.dump();
	}
}
