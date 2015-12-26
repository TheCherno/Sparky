#pragma once

#include "Event.h"

namespace sp { namespace events {

	class SP_API KeyEvent : public Event
	{
	protected:
		int m_KeyCode;
		int m_Count;
	public:
		KeyEvent(int keyCode, Event::Type type);

		inline int GetKeyCode() const { return m_KeyCode; }

		inline static int GetStaticType() { return (int)Event::Type::KEY_PRESSED | (int)Event::Type::KEY_RELEASED; }
	};

	class SP_API KeyPressedEvent : public KeyEvent
	{
	private:
		int m_Repeat;
	public:
		KeyPressedEvent(int button, int repeat);

		inline int GetRepeat() const { return m_Repeat; }

		inline static Type GetStaticType() { return Event::Type::KEY_PRESSED; }
	};

	class SP_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int button);

		inline static Type GetStaticType() { return Event::Type::KEY_RELEASED; }
	};

} }