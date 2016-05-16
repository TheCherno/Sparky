#include "Event.h"

namespace sp { namespace cli { namespace events {

	Event::Event(sp::events::Event* instance)
		: ManagedClass(instance)
	{
	}

	Event::Event(EventType type)
	{
		m_Instance = new sp::events::Event(*(sp::events::Event::Type*)&type);
	}

	EventType Event::GetType()
	{

	}

	bool Event::IsHandled()
	{

	}

	System::String^ Event::ToString()
	{

	}

	System::String^ Event::TypeToString(EventType type)
	{

	}

} } }