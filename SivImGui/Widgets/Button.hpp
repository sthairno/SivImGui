#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class Button : public Container
	{
		SIVIMGUI_BUILDER_HELPER(Button);

		static Button& New(Builder& ctx)
		{
			return ctx.next<Button>(TypeInfo());
		}

		static WidgetTypeInfo TypeInfo()
		{
			return {
				.id = typeid(Button).hash_code(),
				.name = U"Button",
				.generator = [] { return std::make_unique<Button>(); }
			};
		}

	public:

		Button()
			: Container(TypeInfo()) { }

	public:

		Property<ColorF> backgroundColor{ *this, ColorF{ 1.0 } };

		Property<ColorF> flameColor{ *this, ColorF{ 0.67 } };

		Property<ColorF> disabledBackgroundColor{ *this, ColorF{ 0.92 } };

		Property<ColorF> disabledFlameColor{ *this, ColorF{ 0.67 } };

		Property<ColorF> disabledTextColor{ *this, ColorF{ 0.67 } };

		Property<ColorF> mouseOverBackgroundColor{ *this, ColorF{ 0.9, 0.95, 1.0 } };

		Property<ColorF> mouseOverFlameColor{ *this, ColorF{ 0.67 } };

		Property<double> frameThickness{ *this, 1, PropertyFlag::Layout };

		Property<double> roundSize{ *this, 4.8, PropertyFlag::Layout };

	public:

		bool mouseOver() const { return m_mouseOver; }

		bool pressed() const { return m_pressed; }

		bool clicked() const { return m_clicked; }

	protected:

		bool m_mouseOver = false;

		bool m_pressed = false;

		bool m_clicked = false;

		virtual MeasureResult measure() const override
		{
			auto result = layout->measure(visibleChildren());
			result.minSize += Padding{ frameThickness, Max<double>(frameThickness, roundSize) };
			return result;
		}

		virtual Array<RectF> arrange(RectF rect) const override
		{
			return layout->arrange(
				rect - Padding{ frameThickness, Max<double>(frameThickness, roundSize) },
				visibleChildren()
			);
		}

		virtual bool update(RectF rect, bool handled) override
		{
			m_clicked = false;
			m_mouseOver = not handled && rect.mouseOver();

			if (m_mouseOver)
			{
				handled = true;
				if (MouseL.down())
				{
					m_pressed = true;
				}
			}

			if (m_pressed && not MouseL.pressed())
			{
				if (m_mouseOver)
				{
					m_clicked = true;
				}
				m_pressed = false;
			}

			return handled;
		}

		virtual void draw(RectF rect) const override
		{
			RoundRect rrect{ rect, roundSize };

			ColorF backCol;
			ColorF frameCol;

			if (true /*enabled*/)
			{
				if (m_pressed)
				{
					backCol = mouseOverBackgroundColor;
					frameCol = mouseOverFlameColor;
				}
				else if (m_mouseOver)
				{
					backCol = mouseOverBackgroundColor;
					frameCol = mouseOverFlameColor;
				}
				else
				{
					backCol = backgroundColor;
					frameCol = flameColor;
				}
			}
			else
			{
				backCol = disabledBackgroundColor;
				frameCol = disabledFlameColor;
			}

			rrect.draw(backCol);
			rrect.drawFrame(frameThickness, 0, frameCol);

			if (m_mouseOver)
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}
		}
	};
}
