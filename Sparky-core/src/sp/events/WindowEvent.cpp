#include "sp/sp.h"
#include "WindowEvent.h"

namespace sp { namespace events {

	WindowResizeEvent::WindowResizeEvent(uint width, uint height)
		: Event(WindowResizeEvent::GetStaticType()), m_Size(maths::tvec2<uint>(width, height))
	{
	}

	WindowFocusEvent::WindowFocusEvent(bool focus)
		: Event(WindowFocusEvent::GetStaticType()), m_Focus(focus)
	{
	}

	WindowCloseEvent::WindowCloseEvent()
		: Event(WindowCloseEvent::GetStaticType())
	{
	}

	WindowActiveEvent::WindowActiveEvent(bool active)
		: Event(WindowActiveEvent::GetStaticType()), m_Active(active)
	{
	}

	WindowMoveEvent::WindowMoveEvent(uint x, uint y)
		: Event(WindowMoveEvent::GetStaticType()), m_Position(maths::tvec2<uint>(x, y))
	{
	}

} }