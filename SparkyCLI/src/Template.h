#pragma once

#include <sp/app/Window.h>

#include "sp/cli/SparkyCLI.h"

namespace sp { namespace cli {

	public ref class Window : public ManagedClass<sp::Window>
	{
	internal:
		Window(sp::Window* instance);
	public:
		Window();
	};

} }