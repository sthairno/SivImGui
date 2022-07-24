#include "TestWidget.hpp"
#include "../WidgetCore.hpp"
#include "../BuilderContext.hpp"

namespace GUI
{
	class TestWidgetCore : public WidgetCore
	{
		using WidgetCore::WidgetCore;

	public:

		TestWidget controller { *this };

		WidgetController& getController() override { return controller; }

		bool isContainer() const override { return true; }

		const char32_t* getName() const override { return U"TestWidget"; }

	private:

		bool pressed = false;

		int clickCount = 0;

		MeasureResult measureImpl(SizeF availableSize) override
		{
			const Padding padding{ controller.frameThickness };

			auto result = measureContainer(availableSize - padding);
			result.desiredSize += padding;

			return result;
		}

		void arrangeImpl(RectF rect) override
		{
			arrangeContainer(rect - Padding{ controller.frameThickness });
		}

		void drawImpl(RectF rect) override
		{
			rect.draw(ColorF(Palette::Orange, Min(clickCount, 10) / 10.0));
			rect.drawFrame(controller.frameThickness, 0, controller.frameColor);
			for (auto& child : children())
			{
				child->draw();
			}
		}

		void onEvent(Event& e, RectF globalRect) override
		{
			switch (e.type)
			{
			case EventType::MouseDown:
				pressed = true;
				e.handled = true;
				break;
			case EventType::Click:
				if (pressed)
				{
					auto& data = std::get<ClickEventData>(e.data);
					e.handled = true;
					pressed = false;
					if (globalRect.contains(data.upPos))
					{
						clickCount = data.count;
					}
				}
				break;
			}
		}
	};

	TestWidget& Builder::TestWidget(Builder::Context& ctx)
	{
		return ctx.next<TestWidgetCore>().controller;
	}
}
