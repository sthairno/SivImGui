#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class Box : public Container
	{
		SIVIMGUI_BUILDER_HELPER(Box);

		static Box& New(Builder& ctx, ColorF backColor)
		{
			auto& w = ctx.next<Box>(TypeInfo());
			w.backColor = backColor;
			return w;
		}

		static WidgetTypeInfo TypeInfo()
		{
			return {
				.id = typeid(Box).hash_code(),
				.name = U"Box",
				.generator = [] { return std::make_unique<Box>(); },
				.enableMouseOver = false
			};
		}

	public:

		Box()
			: Container(TypeInfo()) { }

	public:

		Property<ColorF> backColor{ *this, Palette::White, PropertyFlag::Layout };

		Property<ColorF> frameColor{ *this, Palette::Black, PropertyFlag::Layout };

		Property<double> frameThickness{ *this, 1, PropertyFlag::Layout };

	protected:

		virtual MeasureResult measure() const override
		{
			auto result = layout->measure(visibleChildren());
			result.minSize += Padding{ frameThickness };
			return result;
		}

		virtual Array<RectF> arrange(RectF rect) const override
		{
			return layout->arrange(
				rect - Padding{ frameThickness },
				visibleChildren()
			);
		}

		virtual void draw(RectF rect) const override
		{
			rect.draw(backColor).drawFrame(frameThickness, 0, frameColor);
		}
	};
}
