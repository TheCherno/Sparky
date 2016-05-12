#include "sp/sp.h"
#include "Input.h"

namespace sp {
	std::map<uint, char> Input::normalChars = std::map<uint, char>();
	std::map<uint, char> Input::shiftChars = std::map<uint, char>();

	InputManager* Input::s_InputManager = nullptr;

	InputManager::InputManager()
	{
		ClearKeys();
		ClearMouseButtons();

		m_MouseGrabbed = true;

		Input::s_InputManager = this;

		Input::normalChars[SP_KEY_0] = '0'; Input::shiftChars[SP_KEY_0] = ')';
		Input::normalChars[SP_KEY_1] = '1'; Input::shiftChars[SP_KEY_1] = '!';
		Input::normalChars[SP_KEY_2] = '2'; Input::shiftChars[SP_KEY_2] = '@';
		Input::normalChars[SP_KEY_3] = '3'; Input::shiftChars[SP_KEY_3] = '#';
		Input::normalChars[SP_KEY_4] = '4'; Input::shiftChars[SP_KEY_4] = '$';
		Input::normalChars[SP_KEY_5] = '5'; Input::shiftChars[SP_KEY_5] = '%';
		Input::normalChars[SP_KEY_6] = '6'; Input::shiftChars[SP_KEY_6] = '^';
		Input::normalChars[SP_KEY_7] = '7'; Input::shiftChars[SP_KEY_7] = '&';
		Input::normalChars[SP_KEY_8] = '8'; Input::shiftChars[SP_KEY_8] = '*';
		Input::normalChars[SP_KEY_9] = '9'; Input::shiftChars[SP_KEY_9] = '(';
		
		Input::normalChars[SP_KEY_A] = 'a'; Input::shiftChars[SP_KEY_A] = 'A';
		Input::normalChars[SP_KEY_B] = 'b'; Input::shiftChars[SP_KEY_B] = 'B';
		Input::normalChars[SP_KEY_C] = 'c'; Input::shiftChars[SP_KEY_C] = 'C';
		Input::normalChars[SP_KEY_D] = 'd'; Input::shiftChars[SP_KEY_D] = 'D';
		Input::normalChars[SP_KEY_E] = 'e'; Input::shiftChars[SP_KEY_E] = 'E';
		Input::normalChars[SP_KEY_F] = 'f'; Input::shiftChars[SP_KEY_F] = 'F';
		Input::normalChars[SP_KEY_G] = 'g'; Input::shiftChars[SP_KEY_G] = 'G';
		Input::normalChars[SP_KEY_H] = 'h'; Input::shiftChars[SP_KEY_H] = 'H';
		Input::normalChars[SP_KEY_I] = 'i'; Input::shiftChars[SP_KEY_I] = 'I';
		Input::normalChars[SP_KEY_J] = 'j'; Input::shiftChars[SP_KEY_J] = 'J';
		Input::normalChars[SP_KEY_K] = 'k'; Input::shiftChars[SP_KEY_K] = 'K';
		Input::normalChars[SP_KEY_L] = 'l'; Input::shiftChars[SP_KEY_L] = 'L';
		Input::normalChars[SP_KEY_M] = 'm'; Input::shiftChars[SP_KEY_M] = 'M';
		Input::normalChars[SP_KEY_N] = 'n'; Input::shiftChars[SP_KEY_N] = 'N';
		Input::normalChars[SP_KEY_O] = 'o'; Input::shiftChars[SP_KEY_O] = 'O';
		Input::normalChars[SP_KEY_P] = 'p'; Input::shiftChars[SP_KEY_P] = 'P';
		Input::normalChars[SP_KEY_Q] = 'q'; Input::shiftChars[SP_KEY_Q] = 'Q';
		Input::normalChars[SP_KEY_R] = 'r'; Input::shiftChars[SP_KEY_R] = 'R';
		Input::normalChars[SP_KEY_S] = 's'; Input::shiftChars[SP_KEY_S] = 'S';
		Input::normalChars[SP_KEY_T] = 't'; Input::shiftChars[SP_KEY_T] = 'T';
		Input::normalChars[SP_KEY_U] = 'u'; Input::shiftChars[SP_KEY_U] = 'U';
		Input::normalChars[SP_KEY_V] = 'v'; Input::shiftChars[SP_KEY_V] = 'V';
		Input::normalChars[SP_KEY_W] = 'w'; Input::shiftChars[SP_KEY_W] = 'W';
		Input::normalChars[SP_KEY_X] = 'x'; Input::shiftChars[SP_KEY_X] = 'X';
		Input::normalChars[SP_KEY_Y] = 'y'; Input::shiftChars[SP_KEY_Y] = 'Y';
		Input::normalChars[SP_KEY_Z] = 'z'; Input::shiftChars[SP_KEY_Z] = 'Z';
		Input::normalChars[SP_KEY_SPACE] = ' ';

		Input::normalChars[SP_KEY_SLASH_FORWARD] = '/';        Input::shiftChars[SP_KEY_SLASH_FORWARD] = '?';
		Input::normalChars[SP_KEY_SLASH_BACK] = '\\';          Input::shiftChars[SP_KEY_SLASH_BACK] = '|';
		Input::normalChars[SP_KEY_SQUARE_BRACKET_OPEN] = '[';  Input::shiftChars[SP_KEY_SQUARE_BRACKET_OPEN] = '{';
		Input::normalChars[SP_KEY_SQUARE_BRACKET_CLOSE] = ']'; Input::shiftChars[SP_KEY_SQUARE_BRACKET_CLOSE] = '}';
		Input::normalChars[SP_KEY_COMMA] = ',';				   Input::shiftChars[SP_KEY_COMMA] = '<';
		Input::normalChars[SP_KEY_PERIOD] = '.';			   Input::shiftChars[SP_KEY_PERIOD] = '>';
		Input::normalChars[SP_KEY_SEMICOLON] = ';';			   Input::shiftChars[SP_KEY_SEMICOLON] = ':';

		Input::normalChars[SP_KEY_EQUAL] = '=';				   Input::shiftChars[SP_KEY_EQUAL] = '+';
		Input::normalChars[SP_KEY_DASH] = '-';				   Input::shiftChars[SP_KEY_DASH] = '_';

		// m_KeyState = spnew bool[MAX_KEYS];
		// m_LastKeyState = spnew bool[MAX_KEYS];
	}

	void InputManager::Update()
	{
		for (int32 i = 0; i < MAX_BUTTONS; i++)
			m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

		memcpy(m_LastKeyState, m_KeyState, MAX_KEYS);
		memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
	}

	void InputManager::ClearKeys()
	{
		for (int32 i = 0; i < MAX_KEYS; i++)
		{
			m_KeyState[i] = false;
			m_LastKeyState[i] = false;
		}
		m_KeyModifiers = 0;
	}

	void InputManager::ClearMouseButtons()
	{
		for (int32 i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
			m_MouseState[i] = false;
			m_MouseClicked[i] = false;
		}
	}

	bool InputManager::IsKeyPressed(uint keycode) const
	{
		// TODO: Log this!
		if (keycode >= MAX_KEYS)
			return false;

		return m_KeyState[keycode];
	}

	bool InputManager::IsMouseButtonPressed(uint button) const
	{
		// TODO: Log this!
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseButtons[button];
	}

	bool InputManager::IsMouseButtonClicked(uint button) const
	{
		// TODO: Log this!
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseClicked[button];
	}

	const maths::vec2& InputManager::GetMousePosition() const
	{
		return m_MousePosition;
	}

	const bool InputManager::IsMouseGrabbed() const
	{
		return m_MouseGrabbed;
	}

	void InputManager::SetMouseGrabbed(bool grabbed)
	{
		m_MouseGrabbed = grabbed;
	}

}