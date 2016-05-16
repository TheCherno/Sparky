#pragma once

#include <sp/app/Window.h>

#include "sp/cli/SparkyCLI.h"

namespace sp { namespace cli {

	public value struct WindowProperties
	{
		uint width, height;
		bool fullscreen;
		bool vsync;
	};

	public ref class Window : public ManagedClass<sp::Window>
	{
	internal:
		Window(sp::Window* instance);
	public:
		Window(System::String^ name, WindowProperties properties);
		void Clear();
		void Update();
		bool Closed();

		void SetTitle(System::String^ title);

		uint GetWidth();
		uint GetHeight();

		void SetVsync(bool enabled);
		bool IsVsync();

		InputManager^ GetInputManager();
	public:
		static Window^ GetWindowClass();
	};

} }