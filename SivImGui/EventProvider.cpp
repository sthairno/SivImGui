#include "EventProvider.hpp"

namespace GUI
{
	void EventProvider::update()
	{
		const Vec2 cursorPos = Cursor::PosF();

		for (char32_t c : TextInput::GetRawInput())
		{
			enqueueEvent(EventType::CharInput, none, CharEventData{
				.data = c
			});
		}

		Array<Input> allMouseInputs = Mouse::GetAllInputs();
		for (auto& input : allMouseInputs)
		{
			if (input.down())
			{
				enqueueEvent(EventType::MouseDown, cursorPos, MouseEventData{
					.button = input,
				});
			}
			else if (input.up())
			{
				enqueueEvent(EventType::MouseUp, cursorPos, MouseEventData{
					.button = input,
				}); 
			}
		}
		updateClickState(cursorPos, allMouseInputs);

		const double wheelV = Mouse::Wheel();
		if (wheelV != 0.0)
		{
			enqueueEvent(EventType::Scroll, cursorPos, ScrollEventData{
				.vec = { wheelV, 0 },
			});
		}
		const double wheelH = Mouse::WheelH();
		if (wheelH != 0.0)
		{
			enqueueEvent(EventType::Scroll, cursorPos, ScrollEventData{
				.vec = { 0, wheelH },
			});
		}

		for (auto& key : Keyboard::GetAllInputs())
		{
			if (key.down())
			{
				enqueueEvent(EventType::KeyDown, none, KeyEventData{
					.key = key
				});
			}
			else if (key.up())
			{
				enqueueEvent(EventType::KeyUp, none, KeyEventData{
					.key = key
				});
			}
		}
	}

	void EventProvider::updateClickState(Vec2 cursorPos, Array<Input>& allInputs)
	{
		if (clickState &&
			clickState->input.up())
		{
			enqueueEvent(EventType::Click, clickState->pos, ClickEventData{
				.button = clickState->input,
				.upPos = cursorPos,
				.count = clickState->count,
			});
		}

		Optional<Input> pressedInput;
		for (Input input : allInputs)
		{
			if (input.pressed())
			{
				if (pressedInput)
				{
					clickState = none;
					return;
				}
				else
				{
					pressedInput = input;
				}
			}
		}
		if (not pressedInput || not pressedInput->down())
		{
			return;
		}

		if (clickState)
		{
			if (clickState->lastStw.elapsed() > multipleClickThreshold ||
				*pressedInput != clickState->input ||
				clickState->pos != cursorPos)
			{
				clickState = none;
			}
		}

		if (clickState)
		{
			clickState->lastStw.restart();
			clickState->count++;
		}
		else
		{
			clickState = detail::ClickState{
				.input = *pressedInput,
				.lastStw = Stopwatch(StartImmediately::Yes),
				.pos = cursorPos,
				.count = 1,
			};
		}
	}
}
