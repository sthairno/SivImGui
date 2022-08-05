#pragma once
#include "Container.hpp"

namespace SivImGui
{
	class Box : public Container
	{
		SIVIMGUI_BUILDER_HELPER(Box);

		static Box& New(Builder& ctx, ColorF backColor)
		{
			auto& w = reinterpret_cast<Box&>(ctx.next(typeid(Box).hash_code(), [&] { return new Box(); }));
			w.backColor = backColor;
			return w;
		}

	public:

		Box() : Container(typeid(Box).hash_code(), U"Box") { }

	public:

		Property<ColorF> backColor{ *this, Palette::White, PropertyFlag::Layout };

		Property<ColorF> frameColor{ *this, Palette::Black, PropertyFlag::Layout };

		Property<double> frameThickness{ *this, 1, PropertyFlag::Layout };

	protected:

		virtual void draw(RectF rect) const override
		{
			rect.draw(backColor).drawFrame(frameThickness, 0, frameColor);
		}
	};
}
