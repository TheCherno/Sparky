#include "Event.h"

namespace sparky { namespace events {

	Event::Event(Type type)
		: m_Type(type), m_Handled(false)
	{
	}


} }