#pragma once

#include <iostream>
#include <sparkygl.h>

#include "../maths/vec2.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "../audio/SoundManager.h"

namespace sparky { namespace graphics {

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

	class Window
	{
	private:
		const char *m_Title;
		int m_Width, m_Height;
		GLFWwindow *m_Window;
		bool m_Closed;

		bool m_Keys[MAX_KEYS];
		bool m_KeyState[MAX_KEYS];
		bool m_KeyTyped[MAX_KEYS];
		bool m_MouseButtons[MAX_BUTTONS];
		bool m_MouseState[MAX_BUTTONS];
		bool m_MouseClicked[MAX_BUTTONS];

		maths::vec2 m_MousePosition;
		bool m_Vsync;
	public:
		Window(const char *name, uint width, uint height);
		~Window();
		void Clear() const;
		void Update();
		void UpdateInput();
		bool Closed() const;

		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight() const { return m_Height; }

		bool IsKeyPressed(uint keycode) const;
		bool IsKeyTyped(uint keycode) const;
		bool IsMouseButtonPressed(uint button) const;
		bool IsMouseButtonClicked(uint button) const;
		const maths::vec2& GetMousePosition() const;

		void SetVsync(bool enabled);
		bool IsVsync() const { return m_Vsync; }
	private:
		bool Init();

		friend void window_resize(GLFWwindow* window, int width, int height);
		friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	};

} }