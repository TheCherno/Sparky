#pragma once

#include <sp/events/Event.h>

#include "sp/cli/SparkyCLI.h"

#ifdef MOUSE_MOVED
#undef MOUSE_MOVED
#endif

namespace sp { namespace cli { namespace events {

	public enum class EventType
	{
		KEY_PRESSED = 1,
		KEY_RELEASED = 2,

		MOUSE_PRESSED = 4,
		MOUSE_RELEASED = 8,
		MOUSE_MOVED = 16,

		WINDOW_RESIZE = 32
	};

	public ref class Event : public ManagedClass<sp::events::Event>
	{
	private:
		Event(sp::events::Event* instance);
	protected:
		Event(EventType type);
	public:
		EventType GetType();
		bool IsHandled();

		virtual System::String^ ToString() override;
	public:
		static System::String^ TypeToString(EventType type);
	};

} } }