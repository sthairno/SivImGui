#pragma once
#include <Siv3D.hpp>
#include "WidgetBase.hpp"

namespace SivImGui
{
	/// @brief UI作成補助クラス
	class Builder
	{
	public:

		/// @brief コンストラクタ
		/// @param root 操作対象ウィジェット
		Builder(WidgetBase& root);

	public:

		WidgetBase& current() const { return *m_stack.back().widget; }

		WidgetBase& next(WidgetBase::TypeID type, std::function<std::unique_ptr<WidgetBase>()> generator);

		template<class WidgetT>
		WidgetT& next(WidgetTypeInfo info)
		{
			return reinterpret_cast<WidgetT&>(next(info.id, info.generator));
		}

		void push(WidgetBase& widget);

		void pop();

		void reset();

		void finalize();

	private:

		WidgetBase& m_root;

		struct State
		{
			WidgetBase* widget;

			WidgetBase::WidgetContainer::const_iterator nextItr;
		};

		std::deque<State> m_stack;
	};
}

/// @brief GUI::Builder補助メンバ関数
/// @param WIDGET_TYPE ウィジェットの型名
#define SIVIMGUI_BUILDER_HELPER(WIDGET_TYPE)\
public:\
template<std::invocable<> Callback> WIDGET_TYPE& operator()(Callback&& f) { builderPush(); f(); builderPop(); return *this; }\
template<std::invocable<WIDGET_TYPE&> Callback> WIDGET_TYPE& operator()(Callback&& f) { builderPush(); f(*this); builderPop(); return *this; }

// example:
//	class Foo : public GUI::Widget
//	{
//		GUI_BUILDER_HELPER(Foo)
//		
//		static Foo& New(GUI::Builder& ctx)
//		{
//			return reinterpret_cast<Foo&>(ctx.next(typeid(Foo).hash_code(), [] { return new Foo(); }));
//		}
//	}
