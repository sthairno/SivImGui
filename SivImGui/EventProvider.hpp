#pragma once
#include "Common.hpp"
#include "Event.hpp"

namespace GUI
{
	namespace detail
	{
		struct ClickState
		{
			Input input;

			Stopwatch lastStw;

			Vec2 pos;

			int count;
		};
	}

	class EventProvider
	{
	public:

		EventProvider(Duration multipleClickThreshold = 400ms)
			: multipleClickThreshold(multipleClickThreshold)
		{ }

		const Duration multipleClickThreshold;

		void update();

		inline Optional<Event> nextEvent()
		{
			if (m_eventQueue.empty())
			{
				return Optional<Event>(none);
			}
			else
			{
				GUI::Event e = m_eventQueue.front();
				m_eventQueue.pop();
				return e;
			}
		}

	private:

		std::queue<Event> m_eventQueue;

		Optional<detail::ClickState> clickState;

		inline void enqueueEvent(EventType type, Optional<Vec2> pos, EventData data)
		{
			m_eventQueue.emplace(Event{
				.type = type,
				.createdFrame = Scene::FrameCount(),
				.handled = false,
				.pos = pos,
				.data = data
			});
		}

		void updateClickState(Vec2 cursorPos, Array<Input>& allInputs);
	};
}
