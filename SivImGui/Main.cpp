#include <Siv3D.hpp> // OpenSiv3D v0.6.4
#include "Root.hpp"

#include "Widgets/Widget.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/SimpleButton.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Box.hpp"
#include "Widgets/Label.hpp"

const Size gridSize{ 5, 6 };

std::unique_ptr<Font> font;
std::unique_ptr<Font> heavyFont;

Array<String> tileChars;

void Reset()
{
	tileChars.clear();
	tileChars.emplace_back(U"");
}

void DrawInfo(Vec2 pos, const SivImGui::WidgetBase& w, bool* mouseOver = nullptr)
{
	const RectF rect = w.rect().movedBy(pos);
	rect.drawFrame(1, 0, Palette::Red);

	bool tmp = false;
	if (not mouseOver)
	{
		mouseOver = &tmp;
	}
	for (auto& child : w.children())
	{
		DrawInfo(rect.pos, *child, mouseOver);
	}

	if (not *mouseOver && rect.mouseOver())
	{
		*mouseOver = true;

		rect.draw(ColorF(Palette::Red, 0.2));
		std::u32string text;
		if (w.isContainer)
		{
			std::u32string_view layoutName = U"";
			switch (w.layout->data.index())
			{
			case 0: layoutName = U"HorizontalLayout"; break;
			case 1: layoutName = U"VerticalLayout"; break;
			case 2: layoutName = U"StackLayout"; break;
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
}

void BuildHeader(SivImGui::Builder& ctx)
{
	SivImGui::Box::New(ctx, ColorF(0.9))([&](SivImGui::Box& b) {
		b.xExpand = true;
		b.minSize = { 10, 50 };
		b.layout = { SivImGui::StackLayout{} };
		b.frameThickness = 0;

		if (SivImGui::SimpleButton::New(ctx, U"Reset").clicked())
		{
			Reset();
		}
		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = { SivImGui::VerticalLayout{
				.padding = { 0.0 },
				.space = 0.0,
				.axisXAlignment = SivImGui::Alignment::Center,
				.axisYAlignment = SivImGui::Alignment::Center,
			} };
			c.xExpand = true;

			SivImGui::Label::New(ctx, U"Layout Demo")([&](SivImGui::Label& l) {
				l.textColor = ColorF(0.1);
			});
		});
	});
}

void BuildTile(SivImGui::Builder& ctx, char32_t chr, bool editing)
{
	SivImGui::Box::New(ctx, editing ? ColorF(1) : ColorF(0.5))([&](SivImGui::Box& b) {
		b.layout = { SivImGui::VerticalLayout{
			.padding = { 0.0 },
			.axisXAlignment = SivImGui::Alignment::Center,
			.axisYAlignment = SivImGui::Alignment::Center,
		} };
		b.minSize = { 60, 60 };
		b.frameThickness = 2;
		b.frameColor = chr ? ColorF(0.5) : ColorF(0.7);

		if (chr)
		{
			auto& l = SivImGui::Label::New(ctx, String(1, chr).uppercase());
			l.font = *heavyFont;
			l.textColor = editing ? Palette::Black : Palette::White;
		}
	});
}

void BuildTiles(SivImGui::Builder& ctx)
{
	SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
		c.layout = { SivImGui::VerticalLayout{
			.padding = { 0.0 },
			.space = 5.0,
		} };
		for (int y : Iota(gridSize.y))
		{
			bool editing = y >= tileChars.size() - 1;
			const String line = y < tileChars.size() ? tileChars[y] : U"";

			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = { SivImGui::HorizontalLayout{
					.padding = { 0.0 },
					.space = 5.0,
				} };

				for (int x : Iota(gridSize.x))
				{
					BuildTile(
						ctx,
						x < line.size() ? line[x] : U'\0',
						editing
					);
				}
			});
		}
	});
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
	Scene::SetBackground(Palette::White);
	Window::SetStyle(WindowStyle::Sizable);

	font = std::make_unique<Font>(14);
	heavyFont = std::make_unique<Font>(28, Typeface::Heavy);

	SivImGui::Root root(std::make_unique<SivImGui::Container>());
	{
		auto& widget = root.getWidget();
		widget.layout = { SivImGui::VerticalLayout{
			.padding = { 0 },
			.space = 0
		} };
		widget.xExpand = true;
		widget.yExpand = true;
	}

	SivImGui::Builder ctx(root.getWidget());
	bool showInfo = false;
	String input;

	Reset();

	while (System::Update())
	{
		root.update();
		ctx.reset();

		/////////

		input.clear();
		BuildHeader(ctx);
		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = { SivImGui::VerticalLayout{
				.padding = { 0, 10, 10 },
				.space = 0,
				.axisXAlignment = SivImGui::Alignment::Center,
			} };
			c.xExpand = true;
			c.yExpand = true;

			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = { SivImGui::VerticalLayout{
					.padding = { 0 },
					.axisYAlignment = SivImGui::Alignment::Center,
				} };
				c.yExpand = true;
				BuildTiles(ctx);
			});
			input += BuildKeyboard(ctx);
			input += TextInput::GetRawInput();
		});

		for (const auto c : input)
		{
			auto& line = tileChars.back();

			if (c == U'\b')
			{
				if (line)
				{
					line.pop_back();
				}
				continue;
			}

			if (c == U'\n' || c == U'\r')
			{
				if (line.size() == gridSize.x &&
					tileChars.size() <= gridSize.y)
				{
					tileChars.emplace_back(U"");
				}
				continue;
			}

			if (U'a' <= c && c <= U'z')
			{
				if (line.size() < gridSize.x)
				{
					line.push_back(c);
				}
			}
		}

		/////////

		ctx.finalize();
		root.layout(Scene::Size());
		root.draw();

		/////////

		// デバッグ情報の表示切替

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

