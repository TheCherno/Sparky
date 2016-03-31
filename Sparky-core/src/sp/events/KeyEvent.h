#pragma once

#include "Event.h"


namespace sp { namespace events {

	class SP_API KeyEvent : public Event
	{
	protected:
		int32 m_KeyCode;
		int32 m_Count;
	public:
		KeyEvent(int32 keyCode, Event::Type type);

		inline int32 GetKeyCode() const { return m_KeyCode; }

		inline static int32 GetStaticType() { return (int32)Event::Type::KEY_PRESSED | (int32)Event::Type::KEY_RELEASED; }
	};

#pragma warning(disable : 4800)
	class SP_API KeyPressedEvent : public KeyEvent
	{
	private:
		int32 m_Repeat;
		int32 m_Modifiers;
	public:
		KeyPressedEvent(int32 button, int32 repeat, int32 modifiers);

		inline int32 GetRepeat() const { return m_Repeat; }
		inline int32 GetModifiers() const { return m_Modifiers; }
		inline bool IsModifier(int32 modifier) const { return (bool)(m_Modifiers & modifier); }

		inline static Type GetStaticType() { return Event::Type::KEY_PRESSED; }
	};
#pragma warning(default : 4800)

	class SP_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32 button);

		inline static Type GetStaticType() { return Event::Type::KEY_RELEASED; }
	};

} }
