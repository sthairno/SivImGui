#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class Button : public Container
	{
		SIVIMGUI_BUILDER_HELPER(Button);

		static Button& New(Builder& ctx)
		{
			return ctx.next<Button>();
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(Button).hash_code(),
				.name = U"Button",
			};
			return info;
		}

	public:

		Button()
			: Container(TypeInfo()) { }

	public:

		Property<ColorF> backgroundColor{ this, ColorF{ 1.0 } };

		Property<ColorF> flameColor{ this, ColorF{ 0.67 } };

		Property<ColorF> disabledBackgroundColor{ this, ColorF{ 0.92 } };

		Property<ColorF> disabledFlameColor{ this, ColorF{ 0.67 } };

		Property<ColorF> disabledTextColor{ this, ColorF{ 0.67 } };

		Property<ColorF> mouseOverBackgroundColor{ this, ColorF{ 0.9, 0.95, 1.0 } };

		Property<ColorF> mouseOverFlameColor{ this, ColorF{ 0.67 } };

		Property<double> frameThickness{ this, 1, PropertyFlag::Layout };

		Property<double> roundSize{ this, 4.8, PropertyFlag::Layout };

	public:

		bool pressed() const { return m_pressed; }

		bool clicked() const { return m_clicked; }

	protected:

		bool m_pressed = false;

		bool m_clicked = false;

		virtual MeasureResult measure() const override
		{
			auto result = Container::measure();
			result.minSize += Padding{ frameThickness, Max<double>(frameThickness, roundSize) };
			return result;
		}

		virtual Array<RectF> arrange(RectF rect) const override
		{
			return Container::arrange(rect - Padding{ frameThickness, Max<double>(frameThickness, roundSize) });
		}

		virtual void update(RectF) override
		{
			m_clicked = false;

			if (mouseOver() && MouseL.down())
			{
				m_pressed = true;
				return;
			}

			if (m_pressed && not MouseL.pressed())
			{
				if (mouseOver())
				{
					m_clicked = true;
				}
				m_pressed = false;
			}
		}

		virtual void draw(RectF rect) const override
		{
			RoundRect rrect{ rect, roundSize };

			ColorF backCol;
			ColorF frameCol;

			if (isEnabled())
			{
				if (m_pressed)
				{
					backCol = mouseOverBackgroundColor;
					frameCol = mouseOverFlameColor;
				}
				else if (mouseOver())
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

			if (mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}
		}
	};
}
