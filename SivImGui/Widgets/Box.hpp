#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class Box : public Container
	{
		SIVIMGUI_BUILDER_HELPER(Box);

		static Box& New(Builder& ctx, ColorF backColor)
		{
			auto& w = ctx.next<Box>();
			w.backColor = backColor;
			return w;
		}

		static const WidgetTypeInfo& TypeInfo()
		{
			const static WidgetTypeInfo info{
				.id = typeid(Box).hash_code(),
				.name = U"Box",
				.enableMouseOver = false
			};
			return info;
		}

	public:

		Box()
			: Container(TypeInfo()) { }

	public:

		Property<ColorF> backColor{ this, Palette::White, PropertyFlag::Layout };

		Property<ColorF> frameColor{ this, Palette::Black, PropertyFlag::Layout };

		Property<int32> frameThickness{ this, 1, PropertyFlag::Layout };

	protected:

		virtual MeasureResult measure() const override
		{
			auto result = Container::measure();
			result.minSize += Padding{ frameThickness };
			return result;
		}

		virtual Array<Rect> arrange(Rect rect) override
		{
			return Container::arrange(rect - Padding{ frameThickness });
		}

		virtual void draw(Rect rect) const override
		{
			rect.draw(backColor).drawFrame(frameThickness, 0, frameColor);
			Container::draw(rect);
		}
	};
}
