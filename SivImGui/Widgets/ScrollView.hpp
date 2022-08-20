#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class ScrollView : public Container
	{
		SIVIMGUI_BUILDER_HELPER(ScrollView);

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

		Property<double> scrollbarSize{ this, 10, PropertyFlag::Layout };

	protected:

		RectF m_contentRect{ 0, 0, 0, 0 };

		MeasureResult m_containerMeasureResult;

		Vector2D<bool> m_scrolling{ false, false };

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
				result.expand.x = false;
				break;
			case Mode::Vertical:
				//     A
				//     |
				//     |
				//     V
				result.minSize.x += scrollbarSize;
				result.minSize.y = scrollbarSize * 2;
				result.expand.y = false;
				break;
			case Mode::Both:
				//     A
				//     |
				//     V
				// <-->#
				result.minSize.x = scrollbarSize * 3;
				result.minSize.y = scrollbarSize * 3;
				result.expand.x = false;
				result.expand.y = false;
				break;
			}
			return result;
		}

		virtual Array<RectF> arrange(RectF rect) override
		{
			MeasureResult result = Container::measure();
			switch (mode)
			{
			case Mode::Horizontal:
				if (result.minSize.x > rect.w)
				{
					//      
					//      
					//      
					// <--->
					m_contentRect.size = {
						result.minSize.x,
						rect.h - scrollbarSize,
					};
				}
				else
				{
					//      
					//      
					//      
					//      
					m_contentRect.size = {
						result.minSize.x,
						rect.h,
					};
				}
				break;
			case Mode::Vertical:
				if (result.minSize.y > rect.h)
				{
					//     A
					//     |
					//     |
					//     V
					m_contentRect.size = {
						rect.w - scrollbarSize,
						result.minSize.y,
					};
				}
				else
				{
					//      
					//      
					//      
					//      
					m_contentRect.size = {
						rect.w,
						result.minSize.y,
					};
				}
				break;
			case Mode::Both:
				m_contentRect.size = result.minSize;
				break;
			default:
				m_contentRect.size = rect.size;
				break;
			}
			return Container::arrange({ 0, 0, m_contentRect.size });
		}

		virtual WidgetBase* hitTest(RectF rect, Vec2 pos) override
		{
			if (not rect.contains(pos))
			{
				return nullptr;
			}

			if (mode == Mode::Horizontal ||
				mode == Mode::Both)
			{
				if (RectF{ Arg::bottomRight = rect.br(), scrollbarSize, rect.h }.contains(pos))
				{
					return this;
				}
			}

			if (mode == Mode::Vertical ||
				mode == Mode::Both)
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
			if (*mode != Mode::None)
			{
				SizeF barAreaSize;
				Optional<RectF> hBarRect, vBarRect;
				switch (*mode)
				{
				case Mode::Horizontal:
					barAreaSize.x = rect.w;
					hBarRect = getHBarRect(rect, barAreaSize.x);
					break;
				case Mode::Vertical:
					barAreaSize.y = rect.h;
					vBarRect = getVBarRect(rect, barAreaSize.y);
					break;
				case Mode::Both:
					barAreaSize.x = rect.w - scrollbarSize;
					barAreaSize.y = rect.h - scrollbarSize;
					hBarRect = getHBarRect(rect, barAreaSize.x);
					vBarRect = getVBarRect(rect, barAreaSize.y);
					break;
				}

				if (mouseOver() && MouseL.down())
				{
					m_scrolling.x |= hBarRect && hBarRect->mouseOver();
					m_scrolling.y |= vBarRect && vBarRect->mouseOver();
				}

				if (not MouseL.pressed())
				{
					m_scrolling = { false, false };
				}

				const Vec2 cursorDelta = Cursor::DeltaF();

				if (hBarRect && m_scrolling.x)
				{
					m_contentRect.x -= cursorDelta.x * (m_contentRect.w - rect.w) / (barAreaSize.x - hBarRect->w);
				}
				m_contentRect.x = Min(Max(m_contentRect.x, rect.w - m_contentRect.w), 0.0);

				if (vBarRect && m_scrolling.y)
				{
					m_contentRect.y -= cursorDelta.y * (m_contentRect.h - rect.h) / (barAreaSize.y - vBarRect->h);
				}
				m_contentRect.y = Min(Max(m_contentRect.y, rect.h - m_contentRect.h), 0.0);
			}

			Transformer2D t(Mat3x2::Translate(m_contentRect.pos), TransformCursor::Yes);
			Container::update(rect);
		}

		virtual void draw(RectF rect) const override
		{
			{
				Transformer2D t(Mat3x2::Translate(m_contentRect.pos), TransformCursor::Yes);
				Container::draw(rect);
			}

			Optional<RectF> hBarRect, vBarRect;
			switch (*mode)
			{
			case Mode::Horizontal:
				hBarRect = getHBarRect(rect, rect.w);
				break;
			case Mode::Vertical:
				vBarRect = getVBarRect(rect, rect.h);
				break;
			case Mode::Both:
				hBarRect = getHBarRect(rect, rect.w - scrollbarSize);
				vBarRect = getVBarRect(rect, rect.h - scrollbarSize);
				break;
			}

			if (hBarRect)
			{
				RoundRect{ *hBarRect, hBarRect->h / 2 }.draw(ColorF(Palette::Black, 0.5));
			}
			if (vBarRect)
			{
				RoundRect{ *vBarRect, vBarRect->w / 2 }.draw(ColorF(Palette::Black, 0.5));
			}
		}

		Optional<RectF> getHBarRect(RectF rect, double width) const
		{
			if (m_contentRect.w <= rect.w)
			{
				return none;
			}

			const double barSize = Max(width * width / m_contentRect.w, scrollbarSize * 2);

			if (barSize >= width)
			{
				return none;
			}

			const double barPos = (width - barSize) * m_contentRect.x / (width - m_contentRect.w);

			return RectF{ Arg::bottomLeft = rect.bl() + Vec2{ barPos, 0 }, barSize, scrollbarSize};
		}

		Optional<RectF> getVBarRect(RectF rect, double height) const
		{
			if (m_contentRect.h <= rect.h)
			{
				return none;
			}

			const double barSize = Max(height * height / m_contentRect.h, scrollbarSize * 2);

			if (barSize >= height)
			{
				return none;
			}

			const double barPos = (height - barSize) * m_contentRect.y / (height - m_contentRect.h);

			return RectF{ Arg::topRight = rect.tr() + Vec2{ 0, barPos }, scrollbarSize, barSize};
		}
	};
}
