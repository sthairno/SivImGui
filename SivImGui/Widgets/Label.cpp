#include "Label.hpp"
#include "../WidgetCore.hpp"
#include "../BuilderContext.hpp"

namespace GUI
{
	class LabelCore : public WidgetCore
	{
		using WidgetCore::WidgetCore;

	public:

		Label controller{ *this };

		WidgetController& getController() override { return controller; }

		bool isContainer() const override { return false; }

		const char32_t* getName() const override { return U"Label"; }

	private:

		MeasureResult measureImpl(SizeF) override
		{
			const Font& font = controller.font.value() ? controller.font.value().value() : getManager().getFont();
			const RectF region = font(controller.text).region();
			[[maybe_unused]] const double baseY = -region.y;
			return { region.size };
		}

		void drawImpl(RectF rect) override
		{
			const Font& font = controller.font.value() ? controller.font.value().value() : getManager().getFont();
			font(controller.text).draw(rect, controller.textColor);
		}
	};

	Label& Builder::Label(Context& ctx, const String& text)
	{
		auto& controller = ctx.next<LabelCore>().controller;
		controller.text = text;
		return controller;
	}
}
