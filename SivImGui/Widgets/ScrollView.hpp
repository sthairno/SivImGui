#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class ScrollView : public Container
	{
		SIVIMGUI_BUILDER_HELPER(ScrollView);

		static constexpr double ButtonScale = 0.6;

		static constexpr double HandleScale = 0.4;

	public:

		enum class Mode : int8
		{
			None = 0b00,
			Horizontal = 0b10,
			Vertical = 0b01,
			Both = 0b11,
		};

		static ScrollView& New(Builder& ctx, Mode mode)
		{
			auto& w = ctx.next<ScrollView>();
			w.mode = mode;
			return w;
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(ScrollView).hash_code(),
				.name = U"ScrollView",
				.enableMouseOver = true
			};
			return info;
		}

	public:

		ScrollView()
			: Container(TypeInfo()) { }

	public:

		Property<Mode> mode{ this, Mode::None, PropertyFlag::Layout };

		Property<double> scrollbarSize{ this, 20, PropertyFlag::Layout };

		Property<SizeF> step{ this, { 20, 20 }, PropertyFlag::Layout };

	protected:

		RectF m_contentRect{ 0, 0, 0, 0 };

		Vector2D<bool> m_scrolling{ false, false };

		Vector2D<bool> m_barVisible{ false, false };

		std::array<RectF, 2> m_barRect;

		virtual MeasureResult measure() const override
		{
			MeasureResult result = Container::measure();
			switch (mode)
			{
			case Mode::Horizontal:
				//      
				//      
				//      
				// <--->
				result.minSize.x = scrollbarSize * 2;
				result.minSize.y += scrollbarSize;
				break;
			case Mode::Vertical:
				//     A
				//     |
				//     |
				//     V
				result.minSize.x += scrollbarSize;
				result.minSize.y = scrollbarSize * 2;
				break;
			case Mode::Both:
				//     A
				//     |
				//     V
				// <-->#
				result.minSize.x = scrollbarSize * 3;
				result.minSize.y = scrollbarSize * 3;
				break;
			}
			return result;
		}

		virtual Array<RectF> arrange(RectF rect) override
		{
			MeasureResult result = Container::measure();

			m_barVisible = {
				result.minSize.x > rect.w,
				result.minSize.y > rect.h,
			};

			if (m_barVisible.x && not m_barVisible.y)
			{
				//      
				//      
				//      
				// <--->
				m_barRect[0] = {
					Arg::bottomLeft = rect.bl() + Vec2{ scrollbarSize, 0 },
					rect.w - scrollbarSize * 2,
					scrollbarSize,
				};
			}
			else if (not m_barVisible.x && m_barVisible.y)
			{
				//     A
				//     |
				//     |
				//     V
				m_barRect[1] = {
					Arg::topRight = rect.tr() + Vec2{ 0, scrollbarSize },
					scrollbarSize,
					rect.h - scrollbarSize * 2,
				};
			}
			else if (m_barVisible.x && m_barVisible.y)
			{
				//     A
				//     |
				//     V
				// <-->#
				m_barRect = {
					RectF {
						Arg::bottomLeft = rect.bl() + Vec2{ scrollbarSize, 0 },
						rect.w - scrollbarSize * 3,
						scrollbarSize,
					},
					RectF {
						Arg::topRight = rect.tr() + Vec2{ 0, scrollbarSize },
						scrollbarSize,
						rect.h - scrollbarSize * 3,
					},
				};
			}

			if (m_barVisible.x)
			{
				rect.h -= scrollbarSize;
			}
			if (m_barVisible.y)
			{
				rect.w -= scrollbarSize;
			}
			m_contentRect = {
				Max(result.minSize.x, rect.w),
				Max(result.minSize.y, rect.h),
			};

			return Container::arrange({ 0, 0, m_contentRect.size });
		}

		virtual WidgetBase* hitTest(RectF rect, Vec2 pos) override
		{
			if (not rect.contains(pos))
			{
				return nullptr;
			}

			if (m_barVisible.x)
			{
				if (RectF{ Arg::bottomRight = rect.br(), scrollbarSize, rect.h }.contains(pos))
				{
					return this;
				}
			}

			if (m_barVisible.y)
			{
				if (RectF{ Arg::bottomRight = rect.br(), rect.w, scrollbarSize }.contains(pos))
				{
					return this;
				}
			}

			if (auto result = hitTestChildren(pos - m_contentRect.pos))
			{
				return result;
			}

			if (enableMouseOver->value_or(typeInfo.enableMouseOver))
			{
				return this;
			}

			return nullptr;
		}

		virtual void update(RectF rect) override
		{
			Optional<RectF> hHandle, vHandle;
			if (m_barVisible.x)
			{
				hHandle = getHHandleRect(rect);
			}
			if (m_barVisible.y)
			{
				vHandle = getVHandleRect(rect);
			}

			if (mouseOver() && MouseL.down())
			{
				if (hHandle && hHandle->mouseOver())
				{
					m_scrolling.x = true;
				}
				if (vHandle && vHandle->mouseOver())
				{
					m_scrolling.y = true;
				}

				if (m_barVisible.x)
				{
					// <
					if (RectF{ m_barRect[0].tl() - Vec2{ scrollbarSize, 0 }, scrollbarSize, scrollbarSize }
						.mouseOver())
					{
						m_contentRect.x += step->x;
					}
					// >
					if (RectF{ m_barRect[0].tr(), scrollbarSize, scrollbarSize }
						.mouseOver())
					{
						m_contentRect.x -= step->x;
					}
				}

				if (m_barVisible.y)
				{
					// A
					if (RectF{ m_barRect[1].tl() - Vec2{ 0, scrollbarSize }, scrollbarSize, scrollbarSize }
						.mouseOver())
					{
						m_contentRect.y += step->y;
					}
					// V
					if (RectF{ m_barRect[1].bl(), scrollbarSize, scrollbarSize }
						.mouseOver())
					{
						m_contentRect.y -= step->y;
					}
				}
			}

			m_scrolling = {
				m_scrolling.x && MouseL.pressed() && hHandle,
				m_scrolling.y && MouseL.pressed() && vHandle,
			};

			const Vec2 cursorDelta = Cursor::DeltaF();

			if (m_scrolling.x)
			{
				m_contentRect.x -= cursorDelta.x * (m_contentRect.w - rect.w) / (m_barRect[0].w - hHandle->w);
			}
			if (m_scrolling.y)
			{
				m_contentRect.y -= cursorDelta.y * (m_contentRect.h - rect.h) / (m_barRect[1].h - vHandle->h);
			}

			m_contentRect.x = Min(Max(m_contentRect.x, rect.w - m_contentRect.w), 0.0);
			m_contentRect.y = Min(Max(m_contentRect.y, rect.h - m_contentRect.h), 0.0);

			Transformer2D t(Mat3x2::Translate(m_contentRect.pos), TransformCursor::Yes);
			Container::update(rect);
		}

		virtual void draw(RectF rect) const override
		{
			{
				Transformer2D t(Mat3x2::Translate(m_contentRect.pos), TransformCursor::Yes);
				Container::draw(rect);
			}

			static constexpr Triangle triangle = Triangle(-Triangle(1.0).boundingRect().center(), 1.0);
			const double btnSize = ButtonScale * scrollbarSize;
			Optional<RectF> hHandle, vHandle;
			if (m_barVisible.x)
			{
				triangle
					.scaled(btnSize)
					.rotatedAt(0, 0, -90_deg)
					.movedBy(m_barRect[0].leftCenter() - Vec2{ scrollbarSize / 2, 0 })
					.draw(ColorF(Palette::Black, 0.5));
				triangle
					.scaled(btnSize)
					.rotatedAt(0, 0, 90_deg)
					.movedBy(m_barRect[0].rightCenter() + Vec2{ scrollbarSize / 2, 0 })
					.draw(ColorF(Palette::Black, 0.5));
				//m_barRect[0].drawFrame(1, Palette::Red);
				hHandle = getHHandleRect(rect);
			}
			if (m_barVisible.y)
			{
				triangle
					.scaled(btnSize)
					.rotatedAt(0, 0, 0_deg)
					.movedBy(m_barRect[1].topCenter() - Vec2{ 0, scrollbarSize / 2 })
					.draw(ColorF(Palette::Black, 0.5));
				triangle
					.scaled(btnSize)
					.rotatedAt(0, 0, 180_deg)
					.movedBy(m_barRect[1].bottomCenter() + Vec2{ 0,  scrollbarSize / 2 })
					.draw(ColorF(Palette::Black, 0.5));
				//m_barRect[1].drawFrame(1, Palette::Red);
				vHandle = getVHandleRect(rect);
			}

			const double handleSize = HandleScale * scrollbarSize;
			if (hHandle)
			{
				RoundRect{ hHandle->scaled(1, HandleScale), handleSize / 2 }
					.draw(ColorF(Palette::Black, 0.5));
			}
			if (vHandle)
			{
				RoundRect{ vHandle->scaled(HandleScale, 1), handleSize / 2 }
					.draw(ColorF(Palette::Black, 0.5));
			}
		}

		Optional<RectF> getHHandleRect(RectF rect) const
		{
			if (m_contentRect.w <= rect.w)
			{
				return none;
			}

			const RectF barRect = m_barRect[0];
			const double handleSize = Max(barRect.w * rect.w / m_contentRect.w, barRect.h * 2);

			if (handleSize >= barRect.w)
			{
				return none;
			}

			const double handlePos = (barRect.w - handleSize) * m_contentRect.x / (rect.w - m_contentRect.w);

			return RectF{ Arg::bottomLeft = barRect.bl() + Vec2{ handlePos, 0 }, handleSize, barRect.h };
		}

		Optional<RectF> getVHandleRect(RectF rect) const
		{
			if (m_contentRect.h <= rect.h)
			{
				return none;
			}

			const RectF barRect = m_barRect[1];
			const double handleSize = Max(barRect.h * rect.h / m_contentRect.h, barRect.w * 2);

			if (handleSize >= barRect.h)
			{
				return none;
			}

			const double handlePos = (barRect.h - handleSize) * m_contentRect.y / (rect.h - m_contentRect.h);

			return RectF{ Arg::topRight = barRect.tr() + Vec2{ 0, handlePos }, barRect.w, handleSize };
		}
	};
}
