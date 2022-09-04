#include <Siv3D.hpp> // OpenSiv3D v0.6.5
#include "GUI.hpp"

#include "Widgets/Widget.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/SimpleButton.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Box.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/ScrollView.hpp"
#include "Widgets/Image.hpp"

const Size gridSize{ 5, 6 };

Array<String> tileChars;

void Reset()
{
	tileChars.clear();
	tileChars.emplace_back(U"");
}

void BuildHeader(SivImGui::Builder& ctx)
{
	SivImGui::Box::New(ctx, ColorF(0.9))([&](SivImGui::Box& b) {
		b.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.space = 0
		};
		b.xExpand = true;
		b.minSize = { 10, 50 };
		b.frameThickness = 0;
		b.enableMouseOver = true;

		auto windowStyle = Window::GetStyle();
		auto& windowState = Window::GetState();

		if (windowStyle == WindowStyle::Frameless)
		{
			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = SivImGui::HorizontalLayout{
					.padding = { 0 },
					.space = 0,
					.horizontalAlignment = SivImGui::Alignment::End
				};
				c.minSize = { 0, windowState.titleBarHeight };

				auto& minimizeBtn = SivImGui::Button::New(ctx)([&](SivImGui::Button& c) {
					c.name = U"wndbtn";
					c.mouseOverBackgroundColor = ColorF(1, 0.5);
					SivImGui::Image::New(ctx, TextureAsset(U"minimize"))
						.diffuse = Palette::Black;
				});
				auto& maximizeBtn = SivImGui::Button::New(ctx)([&](SivImGui::Button& c) {
					c.name = U"wndbtn";
					c.mouseOverBackgroundColor = ColorF(1, 0.5);
					SivImGui::Image::New(ctx, windowState.maximized ? TextureAsset(U"restore") : TextureAsset(U"maximize"))
						.diffuse = Palette::Black;
				});
				auto& closeBtn = SivImGui::Button::New(ctx)([&](SivImGui::Button& c) {
					c.name = U"wndbtn";
					c.mouseOverBackgroundColor = Color(232, 17, 35);
					SivImGui::Image::New(ctx, TextureAsset(U"close"))
						.diffuse = c.mouseOver() ? Palette::White : Palette::Black;
				});

				if (minimizeBtn.clicked())
				{
					if (windowState.minimized)
					{
						Window::Restore();
					}
					else
					{
						Window::Minimize();
					}
				}
				if (maximizeBtn.clicked())
				{
					if (windowState.maximized)
					{
						Window::Restore();
					}
					else
					{
						Window::Maximize();
					}
				}
				if (closeBtn.clicked())
				{
					System::Exit();
				}
			});
		}

		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = SivImGui::StackLayout{
				.padding = { 0 }
			};

			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = SivImGui::HorizontalLayout{ };

				auto& resetBtn = SivImGui::SimpleButton::New(ctx, U"Reset");
				resetBtn.enabled = tileChars[0].size() > 0;
				if (resetBtn.clicked())
				{
					Reset();
				}

				SivImGui::Container::New(ctx).xExpand = true;

				if (SivImGui::SimpleButton::New(ctx,
					windowStyle == WindowStyle::Frameless ? U"Frameless" : U"Normal").clicked())
				{
					if (windowStyle == WindowStyle::Frameless)
					{
						Window::SetStyle(WindowStyle::Sizable);
					}
					else
					{
						Window::SetStyle(WindowStyle::Frameless);
					}
				}
			});

			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = SivImGui::VerticalLayout{
					.padding = { 0 },
					.space = 0,
					.horizontalAlignment = SivImGui::Alignment::Center,
					.verticalAlignment = SivImGui::Alignment::Center,
				};
				c.xExpand = true;

				SivImGui::Label::New(ctx, U"Layout Demo")([&](SivImGui::Label& l) {
					l.textColor = ColorF(0.1);
				});
			});
		});

		static bool moving = false;

		if (b.mouseOver() && MouseL.down())
		{
			moving = true;
		}

		if (moving && not MouseL.pressed())
		{
			moving = false;
		}

		moving &= windowStyle == WindowStyle::Frameless;
		moving &= not windowState.maximized;

		if (moving)
		{
			Window::SetPos(Window::GetPos() + Cursor::ScreenDelta());
		}
	});
}

void BuildTile(SivImGui::Builder& ctx, char32_t chr, bool editing)
{
	SivImGui::Box::New(ctx, editing ? ColorF(1) : ColorF(0.5))([&](SivImGui::Box& b) {
		b.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.horizontalAlignment = SivImGui::Alignment::Center,
			.verticalAlignment = SivImGui::Alignment::Center,
		};
		b.minSize = { 60, 60 };
		b.frameThickness = 2;
		b.frameColor = chr ? ColorF(0.5) : ColorF(0.7);

		if (chr)
		{
			auto& l = SivImGui::Label::New(ctx, String(1, chr).uppercase());
			l.font = FontAsset(U"heavy28");
			l.textColor = editing ? Palette::Black : Palette::White;
		}
	});
}

void BuildTiles(SivImGui::Builder& ctx)
{
	SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
		c.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.space = 5,
		};
		for (int y : Iota(gridSize.y))
		{
			bool editing = y >= tileChars.size() - 1;
			const String line = y < tileChars.size() ? tileChars[y] : U"";

			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = SivImGui::HorizontalLayout{
					.padding = { 0 },
					.space = 5,
				};

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
	auto& key = SivImGui::Button::New(ctx);
	key([&](SivImGui::Button& b) {
		b.minSize = { 60, 46 };
		b.layout = SivImGui::VerticalLayout{
			.padding = { 4, 20 },
			.horizontalAlignment = SivImGui::Alignment::Center,
			.verticalAlignment = SivImGui::Alignment::Center
		};
		SivImGui::Label::New(ctx, label ? label : String(1, chr).uppercase()).textColor = Palette::Black;
	});
	if (key.clicked())
	{
		str.append(chr);
	}
}

String BuildKeyboard(SivImGui::Builder& ctx)
{
	String keyInput;

	SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
		c.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.horizontalAlignment = SivImGui::Alignment::Center
		};

		SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
			c.layout = SivImGui::HorizontalLayout{
				.padding = { 0 }
			};

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
			c.layout = SivImGui::HorizontalLayout{
				.padding = { 0 }
			};

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
			c.layout = SivImGui::HorizontalLayout{
				.padding = { 0 }
			};

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
	Window::SetStyle(WindowStyle::Sizable);

	FontAsset::Register(U"heavy28", 28, Typeface::Heavy);
	TextureAsset::Register(U"minimize", Icon{ Icon::Type::MaterialDesign, 0xF05B0 }, 20);
	TextureAsset::Register(U"maximize", Icon{ Icon::Type::MaterialDesign, 0xF05AF }, 20);
	TextureAsset::Register(U"restore", Icon{ Icon::Type::MaterialDesign, 0xF05B2 }, 20);
	TextureAsset::Register(U"close", Icon{ Icon::Type::MaterialDesign, 0xF05AD }, 20);

	SivImGui::GUI gui(std::make_unique<SivImGui::Box>());
	auto& root = reinterpret_cast<SivImGui::Box&>(gui.getRootWidget());
	{
		root.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.space = 0
		};
		root.frameColor = Palette::Gray;
	}

	SivImGui::Builder ctx(gui.getRootWidget());
	bool showInfo = false;
	String input;

	Reset();

	while (System::Update())
	{
		gui.update();
		ctx.reset();

		/////////

		input.clear();
		BuildHeader(ctx);
		SivImGui::ScrollView::New(ctx, SivImGui::ScrollView::Mode::Both)([&](SivImGui::ScrollView& c) {
			c.layout = SivImGui::VerticalLayout{
				.padding = { 0, 10, 10 },
				.space = 0,
				.horizontalAlignment = SivImGui::Alignment::Center,
			};
			c.xExpand = true;
			c.yExpand = true;

			SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
				c.layout = SivImGui::VerticalLayout{
					.padding = { 10 },
					.verticalAlignment = SivImGui::Alignment::Center,
				};
				c.yExpand = true;
				BuildTiles(ctx);
			});
			input += BuildKeyboard(ctx);
			input += TextInput::GetRawInput();
		});

		for (auto w : gui.findAllWidgets<SivImGui::Button>(U"wndbtn"))
		{
			w->roundSize = 0;
			w->frameThickness = 0;
			w->layout = SivImGui::HorizontalLayout{
				.padding = { 0 },
				.horizontalAlignment = SivImGui::Alignment::Center
			};
			w->minSize = { 40, 0 };
			w->backgroundColor = ColorF::Zero();
			w->disabledBackgroundColor = ColorF::Zero();
		}

		root.frameThickness = Window::GetState().style == WindowStyle::Frameless ? 1 : 0;

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
		gui.layout(Scene::Size());
		{
			const Size windowCurrentSize = Scene::Size();
			const Size windowMinSize = gui.getRootWidget().measuredSize().minSize;
			const Size windowNewSize{
				Max(windowCurrentSize.x, windowMinSize.x),
				Max(windowCurrentSize.y, windowMinSize.y)
			};

			Window::SetMinimumFrameBufferSize(windowMinSize);
			if (not Window::GetState().sizeMove &&
				windowNewSize != windowCurrentSize)
			{
				Window::ResizeVirtual(windowNewSize, Centering::No);
			}
		}
		gui.draw();

		/////////
	}
}

