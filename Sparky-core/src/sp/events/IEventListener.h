#pragma once

#include "Event.h"

namespace sp { namespace events {

	class IEventListener
	{
	public:
		virtual bool OnEvent(const Event& event) = 0;
	};

} }