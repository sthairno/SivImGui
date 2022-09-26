#pragma once
#include <Siv3D.hpp>
#include "WidgetBase.hpp"
#include "BuilderHelper.hpp"

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

		template<class WidgetT>
		BuilderHelper<WidgetT> next()
		{
			return {
				.builder = *this,
				.widget = reinterpret_cast<WidgetT&>(nextImpl(WidgetT::TypeInfo(), [] { return std::make_unique<WidgetT>(); }))
			};
		}

		void reset();

		void finalize();

	private:

		friend void detail::BuilderPush(Builder&, WidgetBase&);

		friend void detail::BuilderPop(Builder&);

		WidgetBase& m_root;

		struct State
		{
			WidgetBase* widget;

			WidgetBase::WidgetContainer::const_iterator nextItr;
		};

		std::deque<State> m_stack;

		WidgetBase& nextImpl(const WidgetTypeInfo& info, std::function<std::unique_ptr<WidgetBase>()> generator);

	public:

		~Builder()
		{
			finalize();
		}
	};
}
