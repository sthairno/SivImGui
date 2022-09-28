#pragma once
#include <Siv3D.hpp>
#include "../Core/UIElement.hpp"
#include "detail/BuilderHelper.hpp"

namespace SivImGui
{
	/// @brief UI作成補助クラス
	class Builder
	{
	public:

		/// @brief コンストラクタ
		/// @param root 操作対象ウィジェット
		Builder(UIElement& root);

	public:

		UIElement& current() const { return *m_stack.back().widget; }

		template<class WidgetT>
		detail::BuilderHelper<WidgetT> next()
		{
			return {
				.builder = *this,
				.widget = reinterpret_cast<WidgetT&>(nextImpl(WidgetT::TypeInfo(), [] { return std::make_unique<WidgetT>(); }))
			};
		}

		void reset();

		void finalize();

	private:

		friend void detail::BuilderPush(Builder&, UIElement&);

		friend void detail::BuilderPop(Builder&);

		UIElement& m_root;

		struct State
		{
			UIElement* widget;

			UIElement::WidgetContainer::const_iterator nextItr;
		};

		std::deque<State> m_stack;

		UIElement& nextImpl(const WidgetTypeInfo& info, std::function<std::unique_ptr<UIElement>()> generator);

	public:

		~Builder()
		{
			finalize();
		}
	};
}
