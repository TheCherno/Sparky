#include "Event.h"

namespace sp { namespace events {

	Event::Event(Type type)
		: m_Type(type), m_Handled(false)
	{
	}


} }