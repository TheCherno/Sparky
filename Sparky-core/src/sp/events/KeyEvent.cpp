#include "sp/sp.h"
#include "KeyEvent.h"

namespace sp { namespace events {

	KeyEvent::KeyEvent(int32 keyCode, Event::Type type)
		: Event(type), m_KeyCode(keyCode)
	{
	}

	KeyPressedEvent::KeyPressedEvent(int32 button, int32 repeat, int32 modifiers)
		: KeyEvent(button, KeyPressedEvent::GetStaticType()), m_Repeat(repeat), m_Modifiers(modifiers)
	{
	}

	KeyReleasedEvent::KeyReleasedEvent(int32 button)
		: KeyEvent(button, KeyReleasedEvent::GetStaticType())
	{
	}

} }