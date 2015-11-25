#pragma once

#include "Event.h"

namespace sp { namespace events {

	class KeyEvent : public Event
	{
	private:
		int m_KeyCode;
	public:
		KeyEvent(int keyCode, Event::Type type);

		inline const int GetKeyCode() const { return m_KeyCode; }
	};

	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_Repeat;
	public:
		KeyPressedEvent(int button, int repeat);

		inline const int GetRepeat() const { return m_Repeat; }
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int button);
	};

} }