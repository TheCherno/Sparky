#include "sp/sp.h"
#include "sp/app/Input.h"
#include "sp/utils/Log.h"

#include <Windows.h>
#include <Windowsx.h>

#include "sp/app/Window.h"
#include "sp/events/Events.h"

#define RAW_BUFFER_SIZE 50

namespace sp {

	using namespace events;

	extern HWND hWnd;
	extern HRAWINPUT rawInputHandle;

	static byte rawBuffer[RAW_BUFFER_SIZE];

	void InputManager::PlatformInit()
	{
		RAWINPUTDEVICE rid[2];

		rid[0].usUsagePage = 1;
		rid[0].usUsage = 6;
		rid[0].dwFlags = 0;
		rid[0].hwndTarget = NULL;

		rid[1].usUsagePage = 1;
		rid[1].usUsage = 2;
		rid[1].dwFlags = 0;
		rid[1].hwndTarget = NULL;
		if (! RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
		{
			SP_ERROR("Failed registering raw input devices!");
		}
	}

	void InputManager::PlatformUpdate()
	{
		m_MouseDelta = m_MouseDeltaCurrent;
		m_MouseDeltaCurrent = 0;
	}

	void InputManager::PlatformUpdateMessage()
	{
		uint bufferSize = RAW_BUFFER_SIZE;
		GetRawInputData(rawInputHandle, RID_INPUT, (LPVOID)rawBuffer, &bufferSize, sizeof(RAWINPUTHEADER));
		
		RAWINPUT* raw = (RAWINPUT*)rawBuffer;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			if ((bool)(m_MouseClicked[SP_MOUSE_1] = raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_1_DOWN)) m_MouseButtons[SP_MOUSE_1] = true;
			else if ((bool)(raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_1_UP))
			{
				m_MouseButtons[SP_MOUSE_1] = false;
				m_EventCallback(MouseReleasedEvent(SP_MOUSE_1, m_MousePosition.x, m_MousePosition.y));
			}

			if ((bool)(m_MouseClicked[SP_MOUSE_2] = raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_2_DOWN)) m_MouseButtons[SP_MOUSE_2] = true;
			else if ((bool)(raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_2_UP))
			{
				m_MouseButtons[SP_MOUSE_2] = false;
				m_EventCallback(MouseReleasedEvent(SP_MOUSE_2, m_MousePosition.x, m_MousePosition.y));
			}

			if ((bool)(m_MouseClicked[SP_MOUSE_3] = raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_3_DOWN)) m_MouseButtons[SP_MOUSE_3] = true;
			else if ((bool)(raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_3_UP))
			{
				m_MouseButtons[SP_MOUSE_3] = false;
				m_EventCallback(MouseReleasedEvent(SP_MOUSE_3, m_MousePosition.x, m_MousePosition.y));
			}

			if ((bool)(m_MouseClicked[SP_MOUSE_4] = raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_4_DOWN)) m_MouseButtons[SP_MOUSE_4] = true;
			else if ((bool)(raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_4_UP))
			{
				m_MouseButtons[SP_MOUSE_4] = false;
				m_EventCallback(MouseReleasedEvent(SP_MOUSE_4, m_MousePosition.x, m_MousePosition.y));
			}

			if ((bool)(m_MouseClicked[SP_MOUSE_5] = raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_5_DOWN)) m_MouseButtons[SP_MOUSE_5] = true;
			else if ((bool)(raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_5_UP))
			{
				m_MouseButtons[SP_MOUSE_5] = false;
				m_EventCallback(MouseReleasedEvent(SP_MOUSE_5, m_MousePosition.x, m_MousePosition.y));
			}

			for (int i = SP_MOUSE_1; i <= SP_MOUSE_5; i++)
			{
				if (m_MouseClicked[i]) m_EventCallback(MousePressedEvent(i, m_MousePosition.x, m_MousePosition.y));
			}

			if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)
			{
				m_MousePosition.x += raw->data.mouse.lLastX;
				m_MousePosition.y += raw->data.mouse.lLastY;

				m_MouseDeltaCurrent.x += raw->data.mouse.lLastX;
				m_MouseDeltaCurrent.y += raw->data.mouse.lLastY;

				m_EventCallback(MouseMovedEvent(m_MousePosition.x, m_MousePosition.y, m_MouseButtons[SP_MOUSE_1]));
			}

			if (raw->data.mouse.ulButtons & RI_MOUSE_WHEEL)
			{
				m_EventCallback(MousePressedEvent(raw->data.mouse.usButtonData == 120 ? SP_MWHEEL_UP : SP_MWHEEL_DOWN, m_MousePosition.x, m_MousePosition.y));
			}
		}

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			if (raw->data.keyboard.Flags & RI_KEY_BREAK)
			{

				m_KeyState[raw->data.keyboard.VKey] = false;
				m_EventCallback(KeyReleasedEvent(raw->data.keyboard.VKey));
			}
			else
			{
				bool repeat = m_KeyState[raw->data.keyboard.VKey];
				m_KeyState[raw->data.keyboard.VKey] = true;
				m_EventCallback(KeyPressedEvent(raw->data.keyboard.VKey, repeat, 0));
			}
		}
	}

	void InputManager::SetMousePosition(const maths::vec2& position)
	{
		POINT pt = { (LONG)position.x, (LONG)position.y };
		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void InputManager::SetMouseCursor(int32 cursor)
	{
		if (cursor == SP_NO_CURSOR)
		{
			RECT wrc;
			GetWindowRect(hWnd, &wrc);
			ClipCursor(&wrc);
			SetCursor(SP_NO_CURSOR);
			while (ShowCursor(false) >= 0);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(true);
		}
	}

	void KeyCallback(InputManager* inputManager, int32 flags, int32 key, uint message)
	{
		// Not implemented, can be removed
	}

	void MouseButtonCallback(InputManager* inputManager, int32 button, int32 x, int32 y)
	{
		// Not implemented, can be removed
	}
}
