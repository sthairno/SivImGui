#pragma once
#include "Common.hpp"

namespace GUI
{
	enum class EventType
	{
		MouseEnter,
		MouseHover,
		MouseExit,
		MouseDown,
		MouseUp,
		KeyDown,
		KeyUp,
		Scroll,
		Click,
		CharInput,
		FocusIn,
		FocusOut,
	};

	struct CharEventData
	{
		char32_t data;
	};

	struct KeyEventData
	{
		Input key;
	};

	struct MouseEventData
	{
		Input button;
	};

	struct ClickEventData
	{
		Input button;

		Vec2 upPos;

		int count;
	};

	struct ScrollEventData
	{
		Vec2 vec;
	};

	using EventData = std::variant<CharEventData, MouseEventData, ScrollEventData, ClickEventData, KeyEventData>;

	struct Event
	{
		EventType type;

		int32 createdFrame;

		bool handled;

		Optional<Vec2> pos;

		EventData data;
	};
}
