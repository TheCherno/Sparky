#include "sp/sp.h"
#include "KeyEvent.h"

namespace sp { namespace events {

	KeyEvent::KeyEvent(int keyCode, Event::Type type)
		: Event(type), m_KeyCode(keyCode)
	{
	}

	KeyPressedEvent::KeyPressedEvent(int button, int repeat)
		: KeyEvent(button, KeyPressedEvent::GetStaticType()), m_Repeat(repeat)
	{
	}

	KeyReleasedEvent::KeyReleasedEvent(int button)
		: KeyEvent(button, KeyReleasedEvent::GetStaticType())
	{
	}

} }