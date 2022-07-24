#include "WidgetController.hpp"
#include "WidgetCore.hpp"
#include "BuilderContext.hpp"

namespace GUI
{
	void WidgetController::makeDirty()
	{
		core.makeDirty();
	}

	void WidgetController::builderPushContainer()
	{
		builder->pushContainer();
	}

	void WidgetController::builderPopContainer()
	{
		builder->popContainer();
	}
}
