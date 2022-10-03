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

namespace SivImGui
{
	Reflection::TypeDB DefaultTypeDB;

	struct StaticInitializer {
	public:
		StaticInitializer()
		{
			RegisterTypes(DefaultTypeDB);
		}

		void RegisterTypes(SivImGui::Reflection::TypeDB& db)
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
			db.push<SivImGui::TabView>()
				.base<SivImGui::Container>()
				.prop(&SivImGui::TabView::font)
				.prop(&SivImGui::TabView::frameThickness)
				.prop(&SivImGui::TabView::tabRound)
				.prop(&SivImGui::TabView::tabPadding)
				.prop(&SivImGui::TabView::tabSpace)
				.prop(&SivImGui::TabView::tabInactiveColor)
				.prop(&SivImGui::TabView::frameColor)
				.prop(&SivImGui::TabView::backgroundColor);
			db.push<SivImGui::Image>()
				.base<SivImGui::Widget>()
				.prop(&SivImGui::Image::texture, true)
				.prop(&SivImGui::Image::fitScale)
				.prop(&SivImGui::Image::diffuse);
			db.push<SivImGui::ScrollView>()
				.base<SivImGui::Container>()
				.prop(&SivImGui::ScrollView::mode)
				.prop(&SivImGui::ScrollView::scrollbarSize)
				.prop(&SivImGui::ScrollView::step);
			db.push<SivImGui::SimpleButton>()
				.base<SivImGui::Widget>()
				.prop(&SivImGui::SimpleButton::text, true);
		}
	};
	StaticInitializer init;

	static UIElement* FindWidgetImpl(UIElement* widget, const StringView name, Optional<size_t> typeId)
	{
		if ((not typeId || widget->typeInfo.id == typeId) &&
			widget->name.value() == name)
		{
			return widget;
		}

		for (const auto& child : widget->children())
		{
			if (auto result = FindWidgetImpl(child.get(), name, typeId))
			{
				return result;
			}
		}

		return nullptr;
	}

	static void FindAllWidgetsImpl(UIElement* widget, Array<UIElement*>& result, const StringView name, Optional<size_t> typeId)
	{
		if ((not typeId || widget->typeInfo.id == typeId) &&
			widget->name.value() == name)
		{
			result.push_back(widget);
		}

		for (const auto& child : widget->children())
		{
			FindAllWidgetsImpl(child.get(), result, name, typeId);
		}
	}

	void GUI::setRootWidget(std::unique_ptr<UIElement>&& widget)
	{
		assert(widget);
		m_root = std::make_unique<Root>(std::move(widget));
		layout();
	}

	void GUI::layout()
	{
		m_availableSize = m_nextAvailableSize;
		m_root->layout(m_availableSize);
	}

	void GUI::update(bool enabled, bool allowMouseOver)
	{
		layout();

		if (m_dirWatcher && m_filePath)
		{
			if (
				m_dirWatcher
				.retrieveChanges()
				.any([this](const FileChange& ev)
			{
				return
					(ev.action == FileAction::Added || ev.action == FileAction::Modified) &&
					FileSystem::FullPath(ev.path) == m_filePath;
			}))
			{
				m_reloadStw.restart();
			}
			if (m_reloadStw.elapsed() >= 0.5s)
			{
				m_reloadStw.reset();
				reload();
			}
		}

		auto& windowState = Window::GetState();
		allowMouseOver &= not windowState.sizeMove && windowState.focused;

		m_root->update(enabled, allowMouseOver);
		layout();
	}

	void GUI::draw() const
	{
		m_root->draw();
	}

	void GUI::reset()
	{
		m_root.reset();
		m_filePath.clear();
		m_dirWatcher = DirectoryWatcher{ };
	}

	bool GUI::load(const XMLElement& elem)
	{
		reset();

		Reflection::XMLParser parser(m_typeDb);

		try
		{
			setRootWidget(parser.parse(elem));
		}
		catch (std::exception)
		{
			reset();
			return false;
		}
		return false;
	}

	bool GUI::load(FilePathView path, EnableHotReload hotReload)
	{
		reset();

		String parentPath = FileSystem::ParentPath(path, 0, m_filePath);

		XMLReader xml(StringView{ m_filePath });
		if (not xml.isOpen())
		{
			reset();
			return false;
		}

		m_dirWatcher = DirectoryWatcher{ parentPath };
		if (not m_dirWatcher.isOpen())
		{
			reset();
			return false;
		}

		Reflection::XMLParser parser(m_typeDb);
		try
		{
			m_root = std::make_unique<Root>(std::move(parser.parse(xml)));
		}
		catch (std::exception)
		{
			reset();
			return false;
		}

		return true;
	}

	bool GUI::reload()
	{
		if (not m_filePath)
		{
			return false;
		}

		XMLReader xml(StringView{ m_filePath });
		if (not xml.isOpen())
		{
			return false;
		}

		Reflection::XMLParser parser(m_typeDb);
		try
		{
			m_root = std::make_unique<Root>(std::move(parser.parse(xml)));
		}
		catch (std::exception)
		{
			return false;
		}

		return true;
	}

	UIElement* GUI::findWidget(const StringView name, Optional<size_t> typeId)
	{
		return FindWidgetImpl(&m_root->widget(), name, typeId);
	}

	Array<UIElement*> GUI::findAllWidgets(const StringView name, Optional<size_t> typeId)
	{
		Array<UIElement*> result;
		FindAllWidgetsImpl(&m_root->widget(), result, name, typeId);
		return result;
	}
}
