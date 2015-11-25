#include "KeyEvent.h"

namespace sp { namespace events {

	KeyEvent::KeyEvent(int keyCode, Event::Type type)
		: Event(type)
	{
	}

	KeyPressedEvent::KeyPressedEvent(int button, int repeat)
		: KeyEvent(button, Event::Type::KEY_PRESSED)
	{
	}

	KeyReleasedEvent::KeyReleasedEvent(int button)
		: KeyEvent(button, Event::Type::KEY_RELEASED)
	{
	}

} }