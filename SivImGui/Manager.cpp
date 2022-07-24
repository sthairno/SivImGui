#include "Manager.hpp"
#include "WidgetCore.hpp"
#include "WidgetController.hpp"
#include "BuilderContext.hpp"

namespace GUI
{
	static void DebugPrint(String& str, WidgetCore& widget, bool last = true, String prefix = U"")
	{
		const bool root = !widget.parent();
		const auto name = widget.getName();
		const auto& children = widget.children();

		str.append(prefix);
		if (last)
		{
			if (not root)
			{
				str.append(U"└");
				prefix += U"　";
			}
		}
		else
		{
			str.append(U"├");
			prefix += U"│";
		}
		str.append(name);
		str.append(U"\n");

		for (int i = 0; i < children.size(); i++)
		{
			DebugPrint(str, *children[i], i == children.size() - 1, prefix);
		}
	}

	namespace detail
	{
		class RootWidget : public WidgetController
		{
			using WidgetController::WidgetController;
		};

		class RootWidgetCore : public WidgetCore
		{
			using WidgetCore::WidgetCore;

		public:

			detail::RootWidget controller{ *this };

			WidgetController& getController() override { return controller; }

			bool isContainer() const override { return true; }

			const char32_t* getName() const override { return U"RootWidget"; }

		public:

			MeasureResult measureImpl(SizeF availableSize) override
			{
				auto res = measureContainer(availableSize);
				return { .desiredSize = availableSize, .isTight = {true, true} };
			}
		};
	}

	Manager::Manager(Font font)
		: m_font(font)
		, m_root(std::make_unique<detail::RootWidgetCore>(*this))
		, m_builder(std::make_unique<Builder::Context>(*m_root))
	{ }

	Manager::~Manager()
	{ }

	void Manager::begin()
	{
		m_builder->reset();
		m_builder->pushContainer();
	}

	void Manager::end()
	{
		m_builder->popContainer();

		m_root->updateLayout(Scene::Size());

		m_eventProvider.update();
		while (auto e = m_eventProvider.nextEvent())
		{
			Print << [&] {
				switch (e->type)
				{
				case EventType::MouseEnter: return U"MouseEnter";
				case EventType::MouseHover: return U"MouseHover";
				case EventType::MouseExit: return U"MouseExit";
				case EventType::MouseDown: return U"MouseDown";
				case EventType::MouseUp: return U"MouseUp";
				case EventType::KeyDown: return U"KeyDown";
				case EventType::KeyUp: return U"KeyUp";
				case EventType::Scroll: return U"Scroll";
				case EventType::Click: return U"Click";
				case EventType::CharInput: return U"CharInput";
				case EventType::FocusIn: return U"FocusIn";
				case EventType::FocusOut: return U"FocusOut";
				}
				return U"";
			}();
			if (e->pos)
			{
				m_root->procPositionedEvent(*e);
			}
		}

		m_root->draw();
	}

	String Manager::dump() const
	{
		String str;
		DebugPrint(str, *m_root);
		return str;
	}

	void Manager::setFont(Font font)
	{
		if (m_font != font)
		{
			m_root->makeAllDirty();
		}
	}
}
