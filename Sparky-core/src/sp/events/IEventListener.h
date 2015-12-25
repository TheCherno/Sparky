#pragma once

#include "Event.h"

namespace sp { namespace events {

	class SP_API IEventListener
	{
	public:
		virtual bool OnEvent(const Event& event) = 0;
	};

} }