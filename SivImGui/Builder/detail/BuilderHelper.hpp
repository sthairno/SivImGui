#pragma once

namespace SivImGui
{
	class Builder;
	class WidgetBase;

	namespace detail
	{
		void BuilderPush(Builder& builder, WidgetBase& widget);

		void BuilderPop(Builder& builder);

		template<class WidgetT>
		struct BuilderHelper
		{
			Builder& builder;

			WidgetT& widget;

			WidgetT& operator()()
			{
				return widget;
			}

			template<std::invocable<> Callback>
			WidgetT& operator()(Callback&& f)
			{
				detail::BuilderPush(builder, widget);
				f();
				detail::BuilderPop(builder);

				return widget;
			}

			template<std::invocable<WidgetT&> Callback>
			WidgetT& operator()(Callback&& f)
			{
				detail::BuilderPush(builder, widget);
				f(widget);
				detail::BuilderPop(builder);

				return widget;
			}
		};
	}
}
