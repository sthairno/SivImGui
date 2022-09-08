#include <Siv3D.hpp> // OpenSiv3D v0.6.5

#include "Core/GUI.hpp"

#include "Widgets/Widget.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/SimpleButton.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Box.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/ScrollView.hpp"
#include "Widgets/Image.hpp"

#include "Reflection/DB.hpp"
#include "Util/XMLParser.hpp"

constexpr Size gridSize = { 5, 6 };
Array<String> tileChars;
SivImGui::Reflection::DB db;

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

void RegisterTypes()
{
	db.push<SivImGui::Widget>();
	db.push<SivImGui::Container>();
	db.push<SivImGui::Box>()
		.base<SivImGui::Container>()
		.prop(&SivImGui::Box::backColor)
		.prop(&SivImGui::Box::frameColor)
		.prop(&SivImGui::Box::frameThickness);
	db.push<SivImGui::Button>()
		.base<SivImGui::Container>()
		.prop(&SivImGui::Button::backgroundColor)
		.prop(&SivImGui::Button::frameColor)
		.prop(&SivImGui::Button::disabledBackgroundColor)
		.prop(&SivImGui::Button::disabledFrameColor)
		.prop(&SivImGui::Button::disabledTextColor)
		.prop(&SivImGui::Button::mouseOverBackgroundColor)
		.prop(&SivImGui::Button::mouseOverFrameColor)
		.prop(&SivImGui::Button::frameThickness)
		.prop(&SivImGui::Button::roundSize);
	db.push<SivImGui::Label>()
		.base<SivImGui::Widget>()
		.prop(&SivImGui::Label::text, true)
		.prop(&SivImGui::Label::textColor)
		.prop(&SivImGui::Label::font)
		.prop(&SivImGui::Label::fitHeight);
}

void Reset()
{
	tileChars.clear();
	tileChars.emplace_back(U"");
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

	ctx.current().layout = SivImGui::VerticalLayout{
		.space = 5,
		.horizontalAlignment = SivImGui::Alignment::Center
	};

	SivImGui::Container::New(ctx)([&](SivImGui::Container& c) {
		c.layout = SivImGui::HorizontalLayout{
			.space = 5
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
			.space = 5
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
			.space = 5
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

	return keyInput;
}

void Main()
{
	Window::SetStyle(WindowStyle::Sizable);

	const String xmlPath = FileSystem::FullPath(U"UI.xml");
	const String directoryPath = FileSystem::ParentPath(xmlPath);
	DirectoryWatcher watcher(directoryPath);
	bool loaded = false;
	Stopwatch reloadTimer(1s, StartImmediately::Yes);

	FontAsset::Register(U"heavy28", 28, Typeface::Heavy);

	SivImGui::GUI gui(std::make_unique<SivImGui::Container>());
	RegisterTypes();

	Reset();

	while (System::Update())
	{
		// XMLの更新を監視, 自動再読み込み

		bool reload = KeyF5.down();
		for (auto& event : watcher.retrieveChanges())
		{
			reload |=
				(event.action == FileAction::Added || event.action == FileAction::Modified) &&
				FileSystem::FullPath(event.path) == xmlPath;
		}
		if (reload)
		{
			loaded = false;
			reloadTimer.restart();
		}
		if (reloadTimer.elapsed() >= 0.5s && not loaded)
		{
			ClearPrint();
			SivImGui::Util::XMLParser parser(db);
			try
			{
				StringView path = xmlPath;
				XMLReader reader(path);
				if (reader)
				{
					gui.setRootWidget(parser.parse(reader));
				}
				else
				{
					Print << U"XMLParseError";
				}
			}
			catch (std::out_of_range ex)
			{
				Print << Unicode::FromUTF8(ex.what());
			}

			loaded = true;
		}

		// 入力/更新処理
		gui.update();

		///// UI更新/アプリ処理 /////

		String input = TextInput::GetRawInput();
		// ソフトキーボード処理
		if (auto keyboard = gui.findWidget<SivImGui::Container>(U"SoftKeyboard"))
		{
			SivImGui::Builder ctx(*keyboard);
			input += BuildKeyboard(ctx);
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
			SivImGui::Builder ctx(*tileArea);
			BuildTiles(ctx);
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
		gui.layout(Scene::Size());

		// ウィンドウの最小サイズを変更
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

		// 描画
		gui.draw();
	}
}
