#include "SimpleSlider.hpp"
#include "../WidgetCore.hpp"
#include "../BuilderContext.hpp"

namespace GUI
{
	class SimpleSliderCore : public WidgetCore
	{
		using WidgetCore::WidgetCore;

	public:

		SimpleSlider controller{ *this };

		WidgetController& getController() override { return controller; }

		bool isContainer() const override { return false; }

		const char32_t* getName() const override { return U"Label"; }

	public:

		bool valueChanged = false;

	private:

		MeasureResult measureImpl(SizeF) override
		{
			return { SimpleGUI::SliderRegion({ 0, 0 }, 0.0, controller.width).size };
		}

		void drawImpl(RectF rect) override
		{
			assert(controller.ref.value());
			valueChanged = SimpleGUI::Slider(*(controller.ref.value()), controller.min, controller.max, rect.pos, controller.width, controller.enabled );
		}
	};

	SimpleSlider& Builder::SimpleSlider(Context& ctx, double& value)
	{
		auto& controller = ctx.next<SimpleSliderCore>().controller;
		controller.ref = &value;
		return controller;
	}

	bool SimpleSlider::valueChanged() const
	{
		return ((SimpleSliderCore&)core).valueChanged;
	}
}
