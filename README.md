# SivImGui

`(Dear ImGui + egui + HTML) / 3`くらいのGUIライブラリ

### 実装例

![sample](assets/sample.png)

#### 事前定義

```cpp
#include <Siv3D.hpp>
#include "Root.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/Image.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	const Font font{ 60 };
	const Texture rocketTex(U"🚀"_emoji);

	// 親要素を作成
	auto root = std::make_unique<SivImGui::Container>();

	// 横幅,縦幅を広げ、子要素を中央に配置
	root->xExpand = true;
	root->yExpand = true;
	root->layout = { SivImGui::HorizontalLayout{
		.axisXAlignment = SivImGui::Alignment::Center,
		.axisYAlignment = SivImGui::Alignment::Center
	} };

	{
		// コンテナを追加
		auto container = std::make_unique<SivImGui::Container>();
		{
			// ラベル"Hello, SivImGui!"を追加
			auto label = std::make_unique<SivImGui::Label>();
			label->text = U"Hello, SivImGui!";
			label->textColor = Palette::Black;
			label->font = font;
			container->addChild(std::move(label));

			// ロケットのテクスチャを追加
			auto image = std::make_unique<SivImGui::Image>();
			image->texture = rocketTex;
			container->addChild(std::move(image));
		}
		root->addChild(std::move(container));
	}

	// GUI環境を作成
	SivImGui::Root gui(std::move(root));

	while (System::Update())
	{
		gui.layout(Scene::Size());
		gui.update();
		gui.draw();
	}
}
```

#### ループ内定義(ImGui風)

```cpp
#include <Siv3D.hpp>
#include "Root.hpp"
#include "Widgets/Container.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/Image.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	const Font font{ 60 };
	const Texture rocketTex(U"🚀"_emoji);

	// GUI環境を作成
	SivImGui::Root gui(std::make_unique<SivImGui::Container>());

	// 横幅,縦幅を広げ、子要素を中央に配置
	auto& root = gui.getWidget();
	root.xExpand = true;
	root.yExpand = true;
	root.layout = { SivImGui::HorizontalLayout{
		.axisXAlignment = SivImGui::Alignment::Center,
		.axisYAlignment = SivImGui::Alignment::Center
	} };

	// 操作対象をrootに設定する
	SivImGui::Builder builder(root);

	while (System::Update())
	{
		gui.update();
		builder.reset();

		// この内部でUIを定義
		// ↓ ↓ ↓ ↓ ↓

		// コンテナの定義
		SivImGui::Container::New(builder)([&] {
			// ラベル"Hello, SivImGui!"を定義
			SivImGui::Label::New(builder, U"Hello, SivImGui!")([&](SivImGui::Label& l) {
				l.textColor = Palette::Black;
				l.font = font;
			});

			// ロケットのテクスチャを定義
			SivImGui::Image::New(builder, rocketTex);
		});

		// ↑ ↑ ↑ ↑ ↑

		builder.finalize();
		gui.layout(Scene::Size());
		gui.draw();
	}
}
```
