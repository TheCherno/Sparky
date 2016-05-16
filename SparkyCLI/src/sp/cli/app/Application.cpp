#include "Window.h"

namespace sp { namespace cli {

	Window::Window(sp::Window* instance)
		: ManagedClass(instance)
	{
	}

	Window::Window(System::String^ name, WindowProperties properties)
	{
		m_Instance = new sp::Window(StringToStdString(name), *(sp::WindowProperties*)&properties);
	}

} }