﻿#include <Siv3D.hpp> // OpenSiv3D v0.6.5

#include "Core.hpp"
#include "Builder.hpp"
#include "Reflection.hpp"

#include "GUI.hpp"

#include "Widgets/Widget.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/SimpleButton.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Box.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/ScrollView.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/TabView.hpp"

constexpr Size gridSize = { 5, 6 };
Array<String> tileChars;

# if SIV3D_PLATFORM(WINDOWS)

# include <Siv3D/Windows/Windows.hpp>

bool LaunchFile(FilePathView path)
{
	const std::wstring file = FileSystem::NativePath(path);

	const INT_PTR result = reinterpret_cast<INT_PTR>(::ShellExecuteW(0, 0, file.c_str(), 0, 0, SW_SHOW));

	if ((result == 0)
		|| (result == ERROR_FILE_NOT_FOUND)
		|| (result == ERROR_PATH_NOT_FOUND)
		|| (result == ERROR_BAD_FORMAT))
	{
		return false;
	}

	return true;
}

# else

bool LaunchFile(FilePathView path)
{
	return false;
}

# endif

void Reset()
{
	tileChars.clear();
	tileChars.emplace_back(U"");
}

void BuildTile(SivImGui::Builder& b, char32_t chr, bool editing)
{
	b.next<SivImGui::Box>()([&](SivImGui::Box& w) {
		w.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.horizontalAlignment = SivImGui::Alignment::Center,
			.verticalAlignment = SivImGui::Alignment::Center,
		};
		w.minSize = { 60, 60 };
		w.frameThickness = 2;
		w.frameColor = chr ? ColorF(0.5) : ColorF(0.7);
		w.backColor = editing ? ColorF(1) : ColorF(0.5);

		if (chr)
		{
			auto& l = b.next<SivImGui::Label>()();
			l.text = String(1, chr).uppercase();
			l.font = FontAsset(U"heavy28");
			l.textColor = editing ? Palette::Black : Palette::White;
		}
	});
}

void BuildTiles(SivImGui::Builder& b)
{
	b.next<SivImGui::Container>()([&](SivImGui::Container& c) {
		c.layout = SivImGui::VerticalLayout{
			.padding = { 0 },
			.space = 5,
		};
		for (int y : Iota(gridSize.y))
		{
			bool editing = y >= tileChars.size() - 1;
			const String line = y < tileChars.size() ? tileChars[y] : U"";

			b.next<SivImGui::Container>()([&](SivImGui::Container& c) {
				c.layout = SivImGui::HorizontalLayout{
					.padding = { 0 },
					.space = 5,
				};

				for (int x : Iota(gridSize.x))
				{
					BuildTile(
						b,
						x < line.size() ? line[x] : U'\0',
						editing
					);
				}
			});
		}
	});
}

void BuildKey(SivImGui::Builder& b, String& str, const char32_t chr, const char32_t* label = nullptr)
{
	auto& key =
		b.next<SivImGui::Button>()([&](SivImGui::Button& w) {
		w.minSize = { 60, 46 };
		w.layout = SivImGui::VerticalLayout{
			.padding = { 4, 20 },
			.horizontalAlignment = SivImGui::Alignment::Center,
			.verticalAlignment = SivImGui::Alignment::Center
		};

		auto& l = b.next<SivImGui::Label>()();
		l.text = label ? label : String(1, chr).uppercase();
		l.textColor = Palette::Black;
	});
	if (key.clicked())
	{
		str.append(chr);
	}
}

String BuildKeyboard(SivImGui::Builder& b)
{
	String keyInput;

	b.current().layout = SivImGui::VerticalLayout{
		.space = 5,
		.horizontalAlignment = SivImGui::Alignment::Center
	};

	b.next<SivImGui::Container>()([&](SivImGui::Container& c) {
		c.layout = SivImGui::HorizontalLayout{
			.space = 5
		};

		BuildKey(b, keyInput, U'q');
		BuildKey(b, keyInput, U'w');
		BuildKey(b, keyInput, U'e');
		BuildKey(b, keyInput, U'r');
		BuildKey(b, keyInput, U't');
		BuildKey(b, keyInput, U'y');
		BuildKey(b, keyInput, U'u');
		BuildKey(b, keyInput, U'i');
		BuildKey(b, keyInput, U'o');
		BuildKey(b, keyInput, U'p');
	});

	b.next<SivImGui::Container>()([&](SivImGui::Container& c) {
		c.layout = SivImGui::HorizontalLayout{
			.space = 5
		};

		BuildKey(b, keyInput, U'a');
		BuildKey(b, keyInput, U's');
		BuildKey(b, keyInput, U'd');
		BuildKey(b, keyInput, U'f');
		BuildKey(b, keyInput, U'g');
		BuildKey(b, keyInput, U'h');
		BuildKey(b, keyInput, U'j');
		BuildKey(b, keyInput, U'k');
		BuildKey(b, keyInput, U'l');
	});

	b.next<SivImGui::Container>()([&](SivImGui::Container& c) {
		c.layout = SivImGui::HorizontalLayout{
			.space = 5
		};

		BuildKey(b, keyInput, U'\n', U"Enter");
		BuildKey(b, keyInput, U'z');
		BuildKey(b, keyInput, U'x');
		BuildKey(b, keyInput, U'c');
		BuildKey(b, keyInput, U'v');
		BuildKey(b, keyInput, U'b');
		BuildKey(b, keyInput, U'n');
		BuildKey(b, keyInput, U'm');
		BuildKey(b, keyInput, U'\b', U"Bksp");
	});

	return keyInput;
}

void Main()
{
	Window::SetStyle(WindowStyle::Sizable);

	const StringView xmlPath = U"UI.xml";

	FontAsset::Register(U"heavy28", 28, Typeface::Heavy);
	TextureAsset::Register(U"mdi-file-edit", { Icon::Type::MaterialDesign, 0xF11E7 }, 30);
	TextureAsset::Register(U"mdi-delete", { Icon::Type::MaterialDesign, 0xF01B4 }, 30);

	SivImGui::GUI gui(xmlPath, SivImGui::EnableHotReload::Yes);

	Reset();
	
	while (System::Update())
	{
		gui.setAvailableSize(Scene::Size());

		// 入力/更新処理
		gui.update();

		///// UI更新/アプリ処理 /////

		String input = TextInput::GetRawInput();
		// ソフトキーボード処理
		if (auto keyboard = gui.findWidget<SivImGui::Container>(U"SoftKeyboard"))
		{
			SivImGui::Builder builder(*keyboard);
			input += BuildKeyboard(builder);
		}

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

		// タイル処理
		if (auto tileArea = gui.findWidget<SivImGui::Container>(U"Tiles"))
		{
			SivImGui::Builder builder(*tileArea);
			BuildTiles(builder);
		}

		// リセットボタン
		if (auto resetBtn = gui.findWidget<SivImGui::Button>(U"ResetBtn"))
		{
			resetBtn->enabled = tileChars[0].size() > 0;
			if (resetBtn->clicked())
			{
				Reset();
			}
		}

		if (auto editBtn = gui.findWidget<SivImGui::Button>(U"EditBtn"))
		{
			if (editBtn->clicked())
			{
				LaunchFile(xmlPath);
			}
		}

		///////////

		// レイアウト更新
		gui.layout();

		// ウィンドウの最小サイズを変更
		{
			const Size windowCurrentSize = Scene::Size();
			const Size windowMinSize = gui.minSize();
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

		// 描画
		gui.draw();
	}
}
