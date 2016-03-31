#include "sp/sp.h"
#include "WindowEvent.h"

namespace sp { namespace events {

	ResizeWindowEvent::ResizeWindowEvent(uint width, uint height)
		: Event(ResizeWindowEvent::GetStaticType()), m_Size(maths::tvec2<uint>(width, height))
	{
	}

} }