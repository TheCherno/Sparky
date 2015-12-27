#include "sp/sp.h"
#include "MouseEvent.h"

namespace sp { namespace events {

	MouseButtonEvent::MouseButtonEvent(int button, float x, float y, Type type)
		: Event(type), m_Button(button), m_Position(maths::vec2(x, y))
	{
	}

	MousePressedEvent::MousePressedEvent(int button, float x, float y)
		: MouseButtonEvent(button, x, y, MousePressedEvent::GetStaticType())
	{
	}

	String MousePressedEvent::ToString() const
	{
		char buffer[256];
		sprintf(buffer, "MouseReleasedEvent: (%d, %f, %f)", GetButton(), GetX(), GetY());
		return String(buffer);
	}

	MouseReleasedEvent::MouseReleasedEvent(int button, float x, float y)
		: MouseButtonEvent(button, x, y, MouseReleasedEvent::GetStaticType())
	{
	}

	MouseMovedEvent::MouseMovedEvent(float x, float y, bool dragged)
		: Event(MouseMovedEvent::GetStaticType()), m_Position(maths::vec2(x, y)), m_Dragged(dragged)
	{
	}

} }