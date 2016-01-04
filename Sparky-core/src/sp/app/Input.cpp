#include "sp/sp.h"
#include "Input.h"

namespace sp {

	InputManager* Input::s_InputManager = nullptr;

	InputManager::InputManager()
	{
		ClearKeys();
		ClearMouseButtons();

		m_MouseGrabbed = true;

		Input::s_InputManager = this;
	}

	void InputManager::Update()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
			m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

		memcpy(m_LastKeyState, m_KeyState, MAX_KEYS);
		memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
	}

	void InputManager::ClearKeys()
	{
		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_KeyState[i] = false;
			m_LastKeyState[i] = false;
		}
		m_KeyModifiers = 0;
	}

	void InputManager::ClearMouseButtons()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
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