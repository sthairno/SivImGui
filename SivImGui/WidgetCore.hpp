#pragma once
#include "Common.hpp"
#include "Manager.hpp"

namespace GUI
{
	class WidgetController;

	struct MeasureResult
	{
		SizeF desiredSize{ 0, 0 };

		struct
		{
			bool x = false;
			bool y = false;
		} isTight;

		static bool Validate(MeasureResult v, SizeF availableSize)
		{
			return
				0 <= v.desiredSize.x && std::isfinite(v.desiredSize.x) &&
				0 <= v.desiredSize.y && std::isfinite(v.desiredSize.y) &&
				(not v.isTight.x || v.desiredSize.x >= availableSize.x) &&
				(not v.isTight.y || v.desiredSize.y >= availableSize.y);
		}
	};

	constexpr inline bool operator==(const MeasureResult& l, const MeasureResult& r)
	{
		return l.desiredSize == r.desiredSize &&
			l.isTight.x == r.isTight.x &&
			l.isTight.y == r.isTight.y;
	}

	class WidgetCore
	{
		using ChildrenContainer = std::vector<std::unique_ptr<WidgetCore>>;

	public:

		WidgetCore(Manager& manager)
			: m_manager(manager)
		{ }

		[[nodiscard]]
		WidgetCore* parent() const noexcept { return m_parent; }

		[[nodiscard]]
		const ChildrenContainer& children() const { return m_children; }

		[[nodiscard]]
		SizeF size() const noexcept { return m_rect.size; }

		[[nodiscard]]
		Vec2 pos() const noexcept { return m_rect.pos; }

		[[nodiscard]]
		RectF rect() const noexcept { return m_rect; }

		[[nodiscard]]
		Manager& getManager() const noexcept { return m_manager; }

		[[nodiscard]]
		Vec2 scrollPos() const noexcept { return m_scrollPos; };

		[[nodiscard]]
		Optional<MeasureResult> measureResult() const noexcept { return m_measureResult; };

		[[nodiscard]]
		Vec2 globalPos() noexcept;

		[[nodiscard]]
		RectF globalRect() noexcept { return { globalPos(), size() }; };

		void builderReset() noexcept
		{
			m_nextItr = m_children.begin();
		}

		template<class ChildType>
		[[nodiscard]]
		ChildType& builderNext()
		{
			assert(isContainer() && "This widget is not container");

			auto currentItr = m_nextItr;

			if (currentItr != m_children.end())
			{
				if (ChildType* child = dynamic_cast<ChildType*>(currentItr->get()))
				{
					m_nextItr++;
					return *child;
				}
				else
				{
					m_children.erase(currentItr, m_children.end());
				}
			}

			ChildType* newChild = new ChildType(m_manager);
			newChild->m_parent = this;
			newChild->m_depth = m_depth + 1;

			m_children.emplace_back(std::unique_ptr<WidgetCore>(newChild));

			m_nextItr = m_children.end();
			m_dirtyLayout = true;
			return *newChild;
		}

		void makeDirty() noexcept;

		void makeAllDirty() noexcept;

		void updateLayout(Optional<SizeF> availableSize = none);

		void draw();

		MeasureResult measure(SizeF availableSize);

		void arrange(Vec2 pos);

		void procPositionedEvent(Event& e, Vec2 offset = { 0, 0 });

	public:

		[[nodiscard]]
		virtual WidgetController& getController() = 0;

		[[nodiscard]]
		virtual bool isContainer() const = 0;

		[[nodiscard]]
		virtual const char32_t* getName() const = 0;

	protected:

		MeasureResult measureContainer(SizeF availableSize);

		void arrangeContainer(RectF rect);

		inline void setScrollPos(Vec2 pos) { m_scrollPos = pos; }

		virtual MeasureResult measureImpl(SizeF availableSize);

		virtual void arrangeImpl(RectF rect);

		virtual void drawImpl(RectF rect);

		virtual void onEvent(Event& e, RectF globalRect);

	private:

		int m_depth = 0;

		Manager& m_manager;

		bool m_dirtyLayout = true;

		WidgetCore* m_parent = nullptr;

		ChildrenContainer m_children;

		ChildrenContainer::iterator m_nextItr;

		Optional<SizeF> m_availableSize;

		Optional<MeasureResult> m_measureResult;

		RectF m_rect{ 0, 0, 0, 0 };

		Vec2 m_scrollPos{ 0, 0 };

	public:

		virtual ~WidgetCore()
		{ }

	};
}
