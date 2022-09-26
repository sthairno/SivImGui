#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class Button : public Container
	{
	public:

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

		SIVIMGUI_PROPERTY(ColorF, backgroundColor, ColorF{ 1.0 });

		SIVIMGUI_PROPERTY(ColorF, frameColor, ColorF{ 0.67 });

		SIVIMGUI_PROPERTY(ColorF, disabledBackgroundColor, ColorF{ 0.92 });

		SIVIMGUI_PROPERTY(ColorF, disabledFrameColor, ColorF{ 0.67 });

		SIVIMGUI_PROPERTY(ColorF, disabledTextColor, ColorF{ 0.67 });

		SIVIMGUI_PROPERTY(ColorF, mouseOverBackgroundColor, ColorF{ 0.9, 0.95, 1.0 });

		SIVIMGUI_PROPERTY(ColorF, mouseOverFrameColor, ColorF{ 0.67 });

		SIVIMGUI_LAYOUT_PROPERTY(int32, frameThickness, 1);

		SIVIMGUI_LAYOUT_PROPERTY(int32, roundSize, 5);

	public:

		bool pressed() const { return m_pressed; }

		bool clicked() const { return m_clicked; }

	protected:

		bool m_pressed = false;

		bool m_clicked = false;

		virtual MeasureResult measure() const override
		{
			auto result = Container::measure();
			result.minSize += Padding{ frameThickness, Max<int32>(frameThickness, roundSize) };
			return result;
		}

		virtual Array<Optional<Rect>> arrange(Rect rect) override
		{
			return Container::arrange(rect - Padding{ frameThickness, Max<int32>(frameThickness, roundSize) });
		}

		virtual void update(Rect rect) override
		{
			Container::update(rect);

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

		virtual void draw(Rect rect) const override
		{
			RoundRect rrect(rect, roundSize);

			ColorF backCol;
			ColorF frameCol;

			if (isEnabled())
			{
				if (m_pressed)
				{
					backCol = mouseOverBackgroundColor;
					frameCol = mouseOverFrameColor;
				}
				else if (mouseOver())
				{
					backCol = mouseOverBackgroundColor;
					frameCol = mouseOverFrameColor;
				}
				else
				{
					backCol = backgroundColor;
					frameCol = frameColor;
				}
			}
			else
			{
				backCol = disabledBackgroundColor;
				frameCol = disabledFrameColor;
			}

			rrect.draw(backCol);
			rrect.drawFrame(frameThickness, 0, frameCol);

			if (mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}

			Container::draw(rect);
		}
	};
}
