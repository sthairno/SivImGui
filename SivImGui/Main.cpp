#include <Siv3D.hpp> // OpenSiv3D v0.6.4
#include "Root.hpp"

#include "Widgets/Widget.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/SimpleButton.hpp"
#include "Widgets/Button.hpp"

std::unique_ptr<Font> font;

bool DrawInfo(Vec2 pos, const SivImGui::WidgetBase& w)
{
	const RectF rect = w.rect().movedBy(pos);
	rect.drawFrame(1, 0, Palette::Red);

	bool mouseOver = false;
	for (auto& child : w.children())
	{
		mouseOver |= DrawInfo(rect.pos, *child);
	}

	if (not mouseOver && rect.mouseOver())
	{
		mouseOver = true;

		rect.draw(ColorF(Palette::Red, 0.2));
		std::u32string text;
		if (w.isContainer)
		{
			std::u32string_view layoutName = U"";
			switch (w.layout->data.index())
			{
			case 0: layoutName = U"HorizontalLayout"; break;
			case 1: layoutName = U"VerticalLayout"; break;
			}
			fmt::format_to(std::back_inserter(text), U"Type: {}(Container)\nRect: {}\nExpand: ({}, {})\nLayout: {}", w.typeName, w.rect(), w.xExpand.value(), w.yExpand.value(), layoutName);
		}
		else
		{
			fmt::format_to(std::back_inserter(text), U"Type: {}(Widget)\nRect: {}\nExpand: ({}, {})", w.typeName, w.rect(), w.xExpand.value(), w.yExpand.value());
		}
		const auto drawableText = (*font)(text);
		const auto cursorPos = Cursor::PosF();

		RectF diagRect{ cursorPos + Vec2{ 20, 20 }, drawableText.region().size + SivImGui::Padding{4} };
		if (diagRect.x + diagRect.w >= Scene::Width())
		{
			diagRect.x = cursorPos.x - diagRect.w - 4;
		}
		if (diagRect.y + diagRect.h >= Scene::Height())
		{
			diagRect.y = cursorPos.y - diagRect.h - 4;
		}
		diagRect.draw(ColorF(1, 0.5)).drawFrame(0, 2, ColorF(1, 0.9));
		drawableText.draw(diagRect - SivImGui::Padding{ 4 }, Palette::Black);
	}

	return mouseOver;
}

void BuildKey(SivImGui::Builder& ctx, String& str, const char32_t chr, const char32_t* label = nullptr)
{
	if (SivImGui::SimpleButton::New(ctx, label ? label : String(1, chr).uppercase()).clicked())
	{
		str.append(chr);
	}
}

String BuildKeyboard(SivImGui::Builder& ctx)
{
	String keyInput;

	SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
		c.layout = { SivImGui::VerticalLayout{
			.padding = { 0 },
			.axisXAlignment = SivImGui::Alignment::Center
		} };

		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = { SivImGui::HorizontalLayout{
				.padding = { 0 }
			} };

			BuildKey(ctx, keyInput, U'q');
			BuildKey(ctx, keyInput, U'w');
			BuildKey(ctx, keyInput, U'e');
			BuildKey(ctx, keyInput, U'r');
			BuildKey(ctx, keyInput, U't');
			BuildKey(ctx, keyInput, U'y');
			BuildKey(ctx, keyInput, U'u');
			BuildKey(ctx, keyInput, U'i');
			BuildKey(ctx, keyInput, U'o');
			BuildKey(ctx, keyInput, U'p');
		});

		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = { SivImGui::HorizontalLayout{
				.padding = { 0 }
			} };

			BuildKey(ctx, keyInput, U'a');
			BuildKey(ctx, keyInput, U's');
			BuildKey(ctx, keyInput, U'd');
			BuildKey(ctx, keyInput, U'f');
			BuildKey(ctx, keyInput, U'g');
			BuildKey(ctx, keyInput, U'h');
			BuildKey(ctx, keyInput, U'j');
			BuildKey(ctx, keyInput, U'k');
			BuildKey(ctx, keyInput, U'l');
		});

		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = { SivImGui::HorizontalLayout{
				.padding = { 0 }
			} };

			BuildKey(ctx, keyInput, U'\n', U"Enter");
			BuildKey(ctx, keyInput, U'z');
			BuildKey(ctx, keyInput, U'x');
			BuildKey(ctx, keyInput, U'c');
			BuildKey(ctx, keyInput, U'v');
			BuildKey(ctx, keyInput, U'b');
			BuildKey(ctx, keyInput, U'n');
			BuildKey(ctx, keyInput, U'm');
			BuildKey(ctx, keyInput, U'\b', U"Bksp");
		});
	});

	return keyInput;
}

void Main()
{
	font = std::make_unique<Font>(14);

	SivImGui::Root root(std::make_unique<SivImGui::Container>());
	root.getWidget().layout = { SivImGui::VerticalLayout{
		.axisXAlignment = SivImGui::Alignment::Center,
		.axisYAlignment = SivImGui::Alignment::Center
	} };
	root.getWidget().xExpand = true;
	root.getWidget().yExpand = true;

	SivImGui::Builder ctx(root.getWidget());

	bool showInfo = false;

	while (System::Update())
	{
		root.update();

		ctx.reset();
		{

			BuildKeyboard(ctx);

		}
		ctx.finalize();

		root.layout(Scene::Size());
		root.draw();

		if (KeySpace.down())
		{
			showInfo = !showInfo;
		}
		if (showInfo)
		{
			DrawInfo({ 0, 0 }, root.getWidget());
		}
	}
}

