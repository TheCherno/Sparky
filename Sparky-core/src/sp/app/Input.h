#pragma once

#include "sp/Common.h"
#include "sp/maths/maths.h"
#include "sp/events/Event.h"

namespace sp {

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

	typedef std::function<void(events::Event& event)> WindowEventCallback;

	class Window;

	class SP_API InputManager
	{
	private:
		friend class Window;
	private:
		bool m_KeyState[MAX_KEYS];
		bool m_LastKeyState[MAX_KEYS];

		bool m_MouseButtons[MAX_BUTTONS];
		bool m_MouseState[MAX_BUTTONS];
		bool m_MouseClicked[MAX_BUTTONS];
		bool m_MouseGrabbed;
		int m_KeyModifiers;

		maths::vec2 m_MousePosition;
		WindowEventCallback m_EventCallback;
	public:
		InputManager();

		inline void SetEventCallback(const WindowEventCallback& eventCallback) { m_EventCallback = eventCallback; }

		void Update();
		void PlatformUpdate();

		bool IsKeyPressed(uint keycode) const;
		bool IsMouseButtonPressed(uint button) const;
		bool IsMouseButtonClicked(uint button) const;

		const maths::vec2& GetMousePosition() const;
		void SetMousePosition(const maths::vec2& position);
		const bool IsMouseGrabbed() const;
		void SetMouseGrabbed(bool grabbed);
		void SetMouseCursor(int cursor);

		void ClearKeys();
		void ClearMouseButtons();
	private:
		friend void KeyCallback(InputManager* inputManager, int flags, int key, uint message);
		friend void MouseButtonCallback(InputManager* inputManager, int button, int x, int y);
	};

	class SP_API Input
	{
	private:
		friend class InputManager;
	private:
		static InputManager* s_InputManager;
	public:
		inline static bool IsKeyPressed(uint keycode) { return s_InputManager->IsKeyPressed(keycode); }
		inline static bool IsMouseButtonPressed(uint button) { return s_InputManager->IsMouseButtonPressed(button); }
		inline static bool IsMouseButtonClicked(uint button) { return s_InputManager->IsMouseButtonClicked(button); }

		inline static const maths::vec2& GetMousePosition() { return s_InputManager->GetMousePosition(); }
	};

}


#define SP_MOUSE_LEFT	  0x00
#define SP_MOUSE_MIDDLE	  0x01
#define SP_MOUSE_RIGHT    0x02

#define SP_NO_CURSOR	  NULL

#define SP_MODIFIER_LEFT_CONTROL	BIT(0)
#define SP_MODIFIER_LEFT_ALT		BIT(1)
#define SP_MODIFIER_LEFT_SHIFT		BIT(2)
#define SP_MODIFIER_RIGHT_CONTROL	BIT(3)
#define SP_MODIFIER_RIGHT_ALT		BIT(4)
#define SP_MODIFIER_RIGHT_SHIFT		BIT(5)

#define SP_KEY_TAB			  0x09

#define SP_KEY_0			  0x30
#define SP_KEY_1			  0x31
#define SP_KEY_2			  0x32
#define SP_KEY_3			  0x33
#define SP_KEY_4			  0x34
#define SP_KEY_5			  0x35
#define SP_KEY_6			  0x36
#define SP_KEY_7			  0x37
#define SP_KEY_8			  0x38
#define SP_KEY_9			  0x39

#define SP_KEY_A			  0x41
#define SP_KEY_B			  0x42
#define SP_KEY_C			  0x43
#define SP_KEY_D			  0x44
#define SP_KEY_E			  0x45
#define SP_KEY_F			  0x46
#define SP_KEY_G			  0x47
#define SP_KEY_H			  0x48
#define SP_KEY_I			  0x49
#define SP_KEY_J			  0x4A
#define SP_KEY_K			  0x4B
#define SP_KEY_L			  0x4C
#define SP_KEY_M			  0x4D
#define SP_KEY_N			  0x4E
#define SP_KEY_O			  0x4F
#define SP_KEY_P			  0x50
#define SP_KEY_Q			  0x51
#define SP_KEY_R			  0x52
#define SP_KEY_S			  0x53
#define SP_KEY_T			  0x54
#define SP_KEY_U			  0x55
#define SP_KEY_V			  0x56
#define SP_KEY_W			  0x57
#define SP_KEY_X			  0x58
#define SP_KEY_Y			  0x59
#define SP_KEY_Z			  0x5A

#define SP_KEY_LBUTTON        0x01
#define SP_KEY_RBUTTON        0x02
#define SP_KEY_CANCEL         0x03
#define SP_KEY_MBUTTON        0x04

#define SP_KEY_ESCAPE         0x1B
#define SP_KEY_SHIFT          0x10
#define SP_KEY_CONTROL        0x11
#define SP_KEY_MENU           0x12
#define SP_KEY_ALT	          SP_KEY_MENU
#define SP_KEY_PAUSE          0x13
#define SP_KEY_CAPITAL        0x14

#define SP_KEY_SPACE          0x20
#define SP_KEY_PRIOR          0x21
#define SP_KEY_NEXT           0x22
#define SP_KEY_END            0x23
#define SP_KEY_HOME           0x24
#define SP_KEY_LEFT           0x25
#define SP_KEY_UP             0x26
#define SP_KEY_RIGHT          0x27
#define SP_KEY_DOWN           0x28
#define SP_KEY_SELECT         0x29
#define SP_KEY_PRINT          0x2A
#define SP_KEY_EXECUTE        0x2B
#define SP_KEY_SNAPSHOT       0x2C
#define SP_KEY_INSERT         0x2D
#define SP_KEY_DELETE         0x2E
#define SP_KEY_HELP           0x2F

#define SP_KEY_NUMPAD0        0x60
#define SP_KEY_NUMPAD1        0x61
#define SP_KEY_NUMPAD2        0x62
#define SP_KEY_NUMPAD3        0x63
#define SP_KEY_NUMPAD4        0x64
#define SP_KEY_NUMPAD5        0x65
#define SP_KEY_NUMPAD6        0x66
#define SP_KEY_NUMPAD7        0x67
#define SP_KEY_NUMPAD8        0x68
#define SP_KEY_NUMPAD9        0x69
#define SP_KEY_MULTIPLY       0x6A
#define SP_KEY_ADD            0x6B
#define SP_KEY_SEPARATOR      0x6C
#define SP_KEY_SUBTRACT       0x6D
#define SP_KEY_DECIMAL        0x6E
#define SP_KEY_DIVIDE         0x6F
#define SP_KEY_F1             0x70
#define SP_KEY_F2             0x71
#define SP_KEY_F3             0x72
#define SP_KEY_F4             0x73
#define SP_KEY_F5             0x74
#define SP_KEY_F6             0x75
#define SP_KEY_F7             0x76
#define SP_KEY_F8             0x77
#define SP_KEY_F9             0x78
#define SP_KEY_F10            0x79
#define SP_KEY_F11            0x7A
#define SP_KEY_F12            0x7B
#define SP_KEY_F13            0x7C
#define SP_KEY_F14            0x7D
#define SP_KEY_F15            0x7E
#define SP_KEY_F16            0x7F
#define SP_KEY_F17            0x80
#define SP_KEY_F18            0x81
#define SP_KEY_F19            0x82
#define SP_KEY_F20            0x83
#define SP_KEY_F21            0x84
#define SP_KEY_F22            0x85
#define SP_KEY_F23            0x86
#define SP_KEY_F24            0x87

#define SP_KEY_NUMLOCK        0x90
#define SP_KEY_SCROLL         0x91

#define SP_KEY_LSHIFT         0xA0
#define SP_KEY_RSHIFT         0xA1
#define SP_KEY_LCONTROL       0xA2
#define SP_KEY_RCONTROL       0xA3
#define SP_KEY_LMENU          0xA4
#define SP_KEY_RMENU          0xA5