#pragma once
#include "Common.hpp"
#include "Builder.hpp"
#include "EventProvider.hpp"
#include "FocusManager.hpp"

namespace GUI
{
	namespace detail
	{
		class RootWidgetCore;
	}

	class Manager
	{
	public:

		Manager()
			: Manager(Font(20))
		{ }

		Manager(Font font);

		~Manager();

		inline Builder::Context& getBuilder() { return *m_builder; }

		inline FocusManager& getFocusManager() { return m_focusManager; }

		inline const Font& getFont() const { return m_font; }

		void begin();

		void end();

		String dump() const;

		void setFont(Font font);

	private:

		Font m_font;

		EventProvider m_eventProvider;

		FocusManager m_focusManager;

		std::unique_ptr<detail::RootWidgetCore> m_root;

		std::unique_ptr<Builder::Context> m_builder;
	};
}
