#pragma once
#include <Siv3D.hpp>
#include "WidgetBase.hpp"

namespace SivImGui
{
	class Root
	{
	public:

		Root(std::unique_ptr<WidgetBase>&& widget)
			: m_widget(std::move(widget))
		{
			assert(m_widget);
		}

		WidgetBase& getWidget() { return *m_widget; }

		void layout(SizeF availableSize);

		bool update(Vec2 pos = { 0, 0 });

		void draw(Vec2 pos = { 0, 0 }) const;

	private:

		std::unique_ptr<WidgetBase> m_widget;
	};
}
