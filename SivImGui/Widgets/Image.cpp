#include "Image.hpp"
#include "../WidgetCore.hpp"
#include "../BuilderContext.hpp"

namespace GUI
{
	class ImageCore : public WidgetCore
	{
		using WidgetCore::WidgetCore;

	public:

		Image controller{ *this };

		WidgetController& getController() override { return controller; }

		bool isContainer() const override { return false; }

		const char32_t* getName() const override { return U"Label"; }

	public:

		bool clicked = false;

	private:

		MeasureResult measureImpl(SizeF) override
		{
			return { controller.texture.value().size() };
		}

		void drawImpl(RectF rect) override
		{
			controller.texture.value()
				.draw(rect.pos, controller.color);
		}
	};

	Image& Builder::Image(Context& ctx, const Texture& texture)
	{
		auto& controller = ctx.next<ImageCore>().controller;
		controller.texture = texture;
		return controller;
	}
}
