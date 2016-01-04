#include "sp/sp.h"
#include "sp/app/Input.h"
#include "sp/utils/Log.h"

#include <Windows.h>
#include <Windowsx.h>

#include "sp/app/Window.h"
#include "sp/events/Events.h"

namespace sp {

	using namespace events;

	extern HWND hWnd;

	void InputManager::PlatformUpdate()
	{
		// Mouse Events
		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(hWnd, &mouse);

		maths::vec2 mousePos = maths::vec2((float)mouse.x, (float)mouse.y);
		if (mousePos != m_MousePosition)
		{
			m_EventCallback(MouseMovedEvent(mousePos.x, mousePos.y, m_MouseButtons[SP_MOUSE_LEFT]));
			m_MousePosition = mousePos;
		}
	}

	void InputManager::SetMousePosition(const maths::vec2& position)
	{
		POINT pt = { (LONG)position.x, (LONG)position.y };
		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void InputManager::SetMouseCursor(int cursor)
	{
		if (cursor == SP_NO_CURSOR)
		{
			SetCursor(SP_NO_CURSOR);
			while (ShowCursor(false) >= 0);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(true);
		}
	}

	void KeyCallback(InputManager* inputManager, int flags, int key, uint message)
	{
		bool pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
		inputManager->m_KeyState[key] = pressed;

		bool repeat = (flags >> 30) & 1;

		int modifier = 0;
		switch (key)
		{
		case SP_KEY_CONTROL:
			modifier = SP_MODIFIER_LEFT_CONTROL;
			break;
		case SP_KEY_ALT:
			modifier = SP_MODIFIER_LEFT_ALT;
			break;
		case SP_KEY_SHIFT:
			modifier = SP_MODIFIER_LEFT_SHIFT;
			break;
		}
		if (pressed)
			inputManager->m_KeyModifiers |= modifier;
		else
			inputManager->m_KeyModifiers &= ~(modifier);

		if (pressed)
			inputManager->m_EventCallback(KeyPressedEvent(key, repeat, inputManager->m_KeyModifiers));
		else
			inputManager->m_EventCallback(KeyReleasedEvent(key));
	}

	void MouseButtonCallback(InputManager* inputManager, int button, int x, int y)
	{
		bool down = false;
		switch (button)
		{
		case WM_LBUTTONDOWN:
			button = SP_MOUSE_LEFT;
			down = true;
			break;
		case WM_LBUTTONUP:
			button = SP_MOUSE_LEFT;
			down = false;
			break;
		case WM_RBUTTONDOWN:
			button = SP_MOUSE_RIGHT;
			down = true;
			break;
		case WM_RBUTTONUP:
			button = SP_MOUSE_RIGHT;
			down = false;
			break;
		case WM_MBUTTONDOWN:
			button = SP_MOUSE_MIDDLE;
			down = true;
			break;
		case WM_MBUTTONUP:
			button = SP_MOUSE_MIDDLE;
			down = false;
			break;
		}

		inputManager->m_MouseButtons[button] = down;
		inputManager->m_MousePosition.x = (float)x;
		inputManager->m_MousePosition.y = (float)y;

		SP_ASSERT(inputManager->m_EventCallback);

		if (down)
			inputManager->m_EventCallback(MousePressedEvent(button, (float)x, (float)y));
		else
			inputManager->m_EventCallback(MouseReleasedEvent(button, (float)x, (float)y));
	}


}