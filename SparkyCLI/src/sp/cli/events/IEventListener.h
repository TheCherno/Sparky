#pragma once

#include <sp/events/IEventListener.h>

#include "sp/cli/SparkyCLI.h"
#include "sp/cli/events/Event.h"

namespace sp { namespace cli { namespace events {

	public interface class IEventListener
	{
	public:
		virtual void OnEvent(sp::cli::events::Event^ e) = 0;
	};

} } }