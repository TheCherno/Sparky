#pragma once

#include "Event.h"

namespace sp { namespace events {

	class SP_API KeyEvent : public Event
	{
	private:
		int m_KeyCode;
	public:
		KeyEvent(int keyCode, Event::Type type);

		inline const int GetKeyCode() const { return m_KeyCode; }
	};

	class SP_API KeyPressedEvent : public KeyEvent
	{
	private:
		int m_Repeat;
	public:
		KeyPressedEvent(int button, int repeat);

		inline const int GetRepeat() const { return m_Repeat; }
	};

	class SP_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int button);
	};

} }