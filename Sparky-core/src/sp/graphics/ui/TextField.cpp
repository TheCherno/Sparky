#include "sp/sp.h"
#include "TextField.h"
#include <iostream>
#include <algorithm>

#include "sp/graphics/FontManager.h"

namespace sp { namespace graphics { namespace ui {

	using namespace maths;

	TextField::TextField(const maths::Rectangle& bounds, const String& text)
		: Widget(bounds), m_Label(text), m_State(false), m_Cursor(0), m_Padding(0.2f), m_SelectStart(-1), m_SelectionColor(0xcf000f5f), m_LowerLimit(0), m_UpperLimit(m_Label.size())
	{
		m_Font = FontManager::Get(24);
	}

	bool TextField::OnMousePressed(events::MousePressedEvent& e)
	{
		if (e.GetButton() == SP_MOUSE_LEFT)
		{
			m_State = true;
			m_Cursor = FindMid(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()));
			m_SelectStart = -1;
		}

		return true;
	}

	bool TextField::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		return false;
	}

	bool TextField::OnMouseMoved(events::MouseMovedEvent& e)
	{
		if (Input::IsMouseButtonPressed(SP_MOUSE_LEFT)) 
		{
			if (m_SelectStart == -1)
			{
				m_SelectStart = m_Cursor;
				return true;
			}
			m_Cursor = FindMid(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()));
		}

		return false;
	}

	bool TextField::OnKeyPressed(events::KeyPressedEvent& e) {
		if (!m_State) return false;
		if (e.GetKeyCode() == SP_KEY_LEFT && m_Cursor > 0)
		{
			if (Input::IsKeyPressed(SP_KEY_SHIFT))
			{
				if (m_SelectStart == -1) m_SelectStart = m_Cursor;
			}
			else
			{
				m_SelectStart = -1;
			}
			if (Input::IsKeyPressed(SP_KEY_CONTROL))
			{ 
				m_Cursor--;
				while (m_Cursor > 0 && m_Label[m_Cursor] != ' ') m_Cursor--;
			}
			else
			{
				m_Cursor--;
			}
		}
		if (e.GetKeyCode() == SP_KEY_RIGHT && m_Cursor < m_Label.size())
		{
			if (Input::IsKeyPressed(SP_KEY_SHIFT))
			{
				if (m_SelectStart == -1) m_SelectStart = m_Cursor;
			}
			else
			{
				m_SelectStart = -1;
			}
			if (Input::IsKeyPressed(SP_KEY_CONTROL))
			{
				m_Cursor++;
				while (m_Cursor < m_Label.size() && m_Label[m_Cursor] != ' ') m_Cursor++;
			}
			else
			{
				m_Cursor++;
			}
		}

		if (Input::IsLetter(e.GetKeyCode()) && !Input::IsKeyPressed(SP_KEY_CONTROL))
		{
			if (m_SelectStart == -1)
			{
				String start = m_Label.substr(0, m_Cursor);
				String end = m_Label.substr(m_Cursor, m_Label.size());
				m_Label = start + Input::GetChar(e.GetKeyCode(), Input::IsKeyPressed(SP_KEY_SHIFT)) + end;
				m_Cursor++;
			}
			else
			{
				ReplaceSelection("" + Input::GetChar(e.GetKeyCode(), Input::IsKeyPressed(SP_KEY_SHIFT)));
				m_Cursor = m_SelectStart + 1;
			}
		}

		if (e.GetKeyCode() == SP_KEY_BACKSPACE && m_Cursor > 0)
		{
			if (m_SelectStart == -1)
			{
				String start = m_Label.substr(0, m_Cursor - 1);
				String end = m_Label.substr(m_Cursor, m_Label.size());
				m_Label = start + end;
				m_Cursor--;
			}
			else
			{
				ReplaceSelection("");
			}
		}

		if (e.GetKeyCode() == SP_KEY_DELETE && m_Cursor < m_Label.size())
		{
			if (m_SelectStart == -1)
			{
				String start = m_Label.substr(0, m_Cursor);
				String end = m_Label.substr(m_Cursor + 1, m_Label.size());
				m_Label = start + end;
			}
			else
			{
				ReplaceSelection("");
			}
		}

		if (e.GetKeyCode() == SP_KEY_HOME)
		{
			if (Input::IsKeyPressed(SP_KEY_SHIFT))
			{
				if (m_SelectStart == -1) m_SelectStart = m_Cursor;
			}
			else
			{
				m_SelectStart = -1;
			}
			m_Cursor = 0;
		}

		if (e.GetKeyCode() == SP_KEY_END) {
			if (Input::IsKeyPressed(SP_KEY_SHIFT))
			{
				if (m_SelectStart == -1) m_SelectStart = m_Cursor;
			}
			else
			{
				m_SelectStart = -1;
			}
			m_Cursor = m_Label.size();
		}

		if (Input::IsKeyPressed(SP_KEY_CONTROL))
		{
			if (e.GetKeyCode() == SP_KEY_A)
			{
				m_Cursor = m_Label.size();
				m_SelectStart = 0;
			}

			if (e.GetKeyCode() == SP_KEY_C)
			{
				if (m_SelectStart != -1)
				{
					int dif = std::max(m_SelectStart, m_Cursor) - std::min(m_SelectStart, m_Cursor);
					String text = m_Label.substr(m_SelectStart, dif);
					Window::SetClipboardContent(text);
				}
			}

			if (e.GetKeyCode() == SP_KEY_X)
			{
				if (m_SelectStart != -1)
				{
					int dif = std::max(m_SelectStart, m_Cursor) - std::min(m_SelectStart, m_Cursor);
					String text = m_Label.substr(m_SelectStart, dif);
					Window::SetClipboardContent(text);

					String start = m_Label.substr(0, std::min(m_SelectStart, m_Cursor));
					String end = m_Label.substr(std::max(m_SelectStart, m_Cursor), m_Label.size());
					m_Label = start + end;
					m_SelectStart = -1;
				}
			}

			if (e.GetKeyCode() == SP_KEY_V)
			{
				String text = Window::GetClipboardContent();
				if (m_SelectStart == -1)
				{
					String start = m_Label.substr(0, m_Cursor);
					String end = m_Label.substr(m_Cursor, m_Label.size());
					m_Label = start + text + end;
					m_Cursor += text.size();
				}
				else
				{
					ReplaceSelection(text);
				}
			}
		}

		if (m_Label.size() == 0) m_UpperLimit = 0;

		if (m_Cursor < m_LowerLimit)
		{
			m_UpperLimit -= m_LowerLimit - m_Cursor;
			m_LowerLimit = m_Cursor;
			while (m_Font->GetWidth(m_Label.substr(m_LowerLimit, m_UpperLimit - m_LowerLimit)) >= (m_Bounds.width - 2 * m_Padding)) m_UpperLimit--;
		}
		if (m_Cursor > m_UpperLimit)
		{
			m_UpperLimit = m_Cursor;
			while (m_Font->GetWidth(m_Label.substr(m_LowerLimit, m_UpperLimit - m_LowerLimit)) >= (m_Bounds.width - 2 * m_Padding)) m_LowerLimit++;
		}
		
		return true;
	}

	void TextField::OnUpdate()
	{
		if (Input::IsMouseButtonPressed(SP_MOUSE_LEFT))
		{
			vec2 mouse(Input::GetMousePosition().x * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - Input::GetMousePosition().y * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
			if (!m_Bounds.Contains(mouse)) m_State = false;
		}
	}

	void TextField::OnRender(Renderer2D& renderer)
	{
		renderer.DrawRect(m_Bounds);
		renderer.FillRect(m_Bounds, 0xcf5f5f5f);
		String text = m_Label.substr(m_LowerLimit, m_UpperLimit - m_LowerLimit);

		float fontWidth = m_Font->GetWidth(m_Label.substr(0, std::max(m_Cursor, m_LowerLimit))) - m_Font->GetWidth(m_Label.substr(0, std::min(m_Cursor, m_LowerLimit)));
		float fontHeight = m_Font->GetHeight(text);
		if (fontHeight < DEFAULT_HEIGHT) fontHeight = DEFAULT_HEIGHT;

		float sStart = m_Font->GetWidth(text.substr(0, m_SelectStart));
		float sPos1 = m_Bounds.x + sStart + m_Padding;
		float sPos2 = fontWidth - sStart;
		if (m_SelectStart != -1 && sPos2 > 0)
		{
			renderer.FillRect(sPos1,
							  m_Bounds.y + m_Bounds.height / 2 - fontHeight / 1.75f,
							  sPos2,
							  2 * (fontHeight / 1.75f),
			m_SelectionColor);
		}
		else if (m_SelectStart != -1 && sPos2 < 0)
		{
			renderer.FillRect(sPos1 + sPos2,
							  m_Bounds.y + m_Bounds.height / 2 - fontHeight / 1.75f,
							  -sPos2,
							  2 * (fontHeight / 1.75f),
			m_SelectionColor);
		}

//		renderer.FillRect(Rectangle(m_Bounds.x + m_Padding, m_Bounds.y, m_Bounds.width - 2 * m_Padding, m_Bounds.height), 0xcf000000);
		renderer.DrawString(text, m_Bounds.position + vec2(m_Padding, m_Bounds.height / 2 - fontHeight / 4), *m_Font);
		if (m_State)
			renderer.DrawLine(m_Bounds.x + fontWidth + m_Padding,
				m_Bounds.y + m_Bounds.height / 2 - fontHeight / 1.75f,
				m_Bounds.x + fontWidth + m_Padding,
				m_Bounds.y + m_Bounds.height / 2 + fontHeight / 1.75f,
			0xcf000000);
	}

	int TextField::FindMid(float x) {
		float dif = x - (m_Bounds.x + m_Padding);

		int low = 0, mid, high = m_Label.size();

		while (low <= high)
		{
			mid = round((high + low) / 2);

			float midLength = m_Font->GetWidth(m_Label.substr(0, mid));

			if (midLength < dif) low = mid + 1;
			else if (midLength > dif) high = mid - 1;
		}

		float l1 = m_Font->GetWidth(m_Label.substr(0, mid)) - dif;
		float l2 = m_Font->GetWidth(m_Label.substr(0, mid - 1)) - dif;
		float l3 = m_Font->GetWidth(m_Label.substr(0, mid + 1)) - dif;

		if (abs(l1) > abs(l2)) mid--;
		if (abs(l1) > abs(l3)) mid++;

		return mid;
	}

	void TextField::ReplaceSelection(String text) 
	{
		if (m_SelectStart == -1) return;
		int low = m_SelectStart <= m_Cursor ? m_SelectStart : m_Cursor;
		int high = m_SelectStart > m_Cursor ? m_SelectStart : m_Cursor;

		String start = m_Label.substr(0, low);
		String end = m_Label.substr(high, m_Label.size());
		m_Label = start + text + end;
		m_Cursor = std::min(m_SelectStart, m_Cursor) + text.size();
		m_SelectStart = -1;
	}

} } }