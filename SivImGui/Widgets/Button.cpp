#include "Button.hpp"
#include "../WidgetCore.hpp"
#include "../BuilderContext.hpp"

namespace GUI
{
	class ButtonCore : public WidgetCore
	{
		using WidgetCore::WidgetCore;

	public:

		Button controller{ *this };

		WidgetController& getController() override { return controller; }

		bool isContainer() const override { return true; }

		const char32_t* getName() const override { return U"Button"; }

	public:

		bool clicked = false;

		bool dblClicked = false;

	private:

		bool m_pressed = false;

		MeasureResult measureImpl(SizeF availableSize) override
		{
			const Padding padding{ controller.frameThickness };

			auto result = measureContainer(availableSize - padding);
			result.desiredSize += padding;

			return result;
		}

		void arrangeImpl(RectF rect) override
		{
			const Padding padding{ controller.frameThickness };

			arrangeContainer(rect - padding);
		}

		void drawImpl(RectF rect) override
		{
			RoundRect rrect{ rect, controller.roundSize };

			const bool mouseOver = rect.mouseOver();
			ColorF backCol;
			ColorF frameCol;

			if (controller.enabled)
			{
				if (m_pressed)
				{
					backCol = controller.mouseOverBackgroundColor;
					frameCol = controller.mouseOverFlameColor;
				}
				else if (mouseOver)
				{
					backCol = controller.mouseOverBackgroundColor;
					frameCol = controller.mouseOverFlameColor;
				}
				else
				{
					backCol = controller.backgroundColor;
					frameCol = controller.flameColor;
				}
			}
			else
			{
				backCol = controller.disabledBackgroundColor;
				frameCol = controller.disabledFlameColor;
			}

			rrect.draw(backCol);
			rrect.drawFrame(controller.frameThickness, 0, frameCol);

			for (auto& child : children())
			{
				child->draw();
			}
		}

		void onEvent(Event& e, RectF) override
		{
			switch (e.type)
			{
			case EventType::MouseDown:
				e.handled = true;
				m_pressed = true;
				break;
			case EventType::Click:
				if (m_pressed)
				{
					e.handled = true;
					m_pressed = false;
				}
				break;
			}
		}
	};

	Button& Builder::Button(Context& ctx)
	{
		return ctx.next<ButtonCore>().controller;
	}

	bool Button::clicked() const
	{
		return ((ButtonCore&)core).clicked;
	}

	bool Button::dblClicked() const
	{
		return ((ButtonCore&)core).dblClicked;
	}
}
