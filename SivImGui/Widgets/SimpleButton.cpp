#include "SimpleButton.hpp"
#include "../WidgetCore.hpp"
#include "../BuilderContext.hpp"

namespace GUI
{
	class SimpleButtonCore : public WidgetCore
	{
		using WidgetCore::WidgetCore;

	public:

		SimpleButton controller{ *this };

		WidgetController& getController() override { return controller; }

		bool isContainer() const override { return false; }

		const char32_t* getName() const override { return U"SimpleButton"; }

	public:

		bool clicked = false;

	private:

		MeasureResult measureImpl(SizeF) override
		{
			return { SimpleGUI::ButtonRegion(controller.label, { 0, 0 }, controller.width).size };
		}

		void drawImpl(RectF rect) override
		{
			clicked = SimpleGUI::Button(controller.label, rect.pos, controller.width, controller.enabled);
		}

		void onEvent(Event& e, RectF) override
		{
			switch (e.type)
			{
			case EventType::MouseDown:
				e.handled = true;
				break;
			}
		}
	};

	SimpleButton& Builder::SimpleButton(Context& ctx, const String& label)
	{
		auto& controller = ctx.next<SimpleButtonCore>().controller;
		controller.label = label;
		return controller;
	}

	bool SimpleButton::clicked() const
	{
		return ((SimpleButtonCore&)core).clicked;
	}
}
