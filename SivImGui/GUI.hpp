#pragma once
#include <Siv3D.hpp>
#include "Core.hpp"
#include "Reflection.hpp"

namespace SivImGui
{
	using EnableHotReload = YesNo<struct EnableHotReload_tag>;

	extern Reflection::TypeDB DefaultTypeDB;

	class GUI
	{
	public:

		GUI() { }

		GUI(std::unique_ptr<UIElement>&& widget)
		{
			setRootWidget(std::move(widget));
		}

		GUI(const XMLElement& elem, const Reflection::TypeDB& db = DefaultTypeDB)
			: m_typeDb(db)
		{
			load(elem);
		}

		GUI(FilePathView path, EnableHotReload hotReload = EnableHotReload::No, const Reflection::TypeDB& db = DefaultTypeDB)
			: m_typeDb(db)
		{
			load(path, hotReload);
		}

		bool loaded() const { return !!m_root; }

		explicit operator bool() const { return loaded(); }

		UIElement& rootWidget() const
		{
			assert(m_root);
			return m_root->widget();
		}

		Size availableSize() const { return m_availableSize; }

		bool isEnabled() const
		{
			return m_root ? rootWidget().isEnabled() : false;
		}

		Size size() const
		{
			return m_root ? rootWidget().size() : Size::Zero();
		}

		Size minSize() const
		{
			return m_root ? rootWidget().measuredSize().minSize : Size::Zero();
		}

		void setAvailableSize(Size size)
		{
			m_nextAvailableSize = size;
		}

		void setRootWidget(std::unique_ptr<UIElement>&& widget);

		void layout();

		void update(bool enabled = true, bool allowMouseOver = true);

		void draw() const;

		void reset();

		bool load(const XMLElement& elem);

		bool load(FilePathView path, EnableHotReload hotReload = EnableHotReload::Yes);

		bool reload();

		template<class WidgetT>
		WidgetT* findWidget(const StringView name = U"")
		{
			return reinterpret_cast<WidgetT*>(findWidget(name, WidgetT::TypeInfo().id));
		}

		UIElement* findWidget(const StringView name, Optional<size_t> typeId = none);

		template<class WidgetT>
		Array<WidgetT*> findAllWidgets(const StringView name = U"")
		{
			return findAllWidgets(name, WidgetT::TypeInfo().id)
				.map([](UIElement* w) { return reinterpret_cast<WidgetT*>(w); });
		}

		Array<UIElement*> findAllWidgets(const StringView name, Optional<size_t> typeId = none);

	private:

		std::unique_ptr<Root> m_root;

		FilePath m_filePath;

		DirectoryWatcher m_dirWatcher;

		Reflection::TypeDB m_typeDb;

		Size m_nextAvailableSize{ 0, 0 };

		Size m_availableSize{ 0, 0 };

		Stopwatch m_reloadStw;
	};
}
