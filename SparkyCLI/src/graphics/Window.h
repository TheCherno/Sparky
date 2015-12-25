#pragma once

#include "../SparkyCLI.h"
#include "../maths/Vector2.h"

#include <graphics/Window.h>

namespace SparkyCLI {

	public ref class Window : public ManagedClass<sparky::graphics::Window>
	{
	private:
		Window(sparky::graphics::Window* instance);
	public:
		Window(System::String^ name, System::UInt32 width, System::UInt32 height);
		void Clear();
		void Update();
		void UpdateInput();
		bool Closed();

		inline uint GetWidth();
		inline uint GetHeight();

		bool IsKeyPressed(System::UInt32 keycode);
		bool IsKeyTyped(System::UInt32 keycode);
		bool IsMouseButtonPressed(System::UInt32 button);
		bool IsMouseButtonClicked(System::UInt32 button);
		Vector2^ GetMousePosition();

		void SetVsync(bool enabled);
		bool IsVsync();
	};

}