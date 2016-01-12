#include "sp/sp.h"
#include "Button.h"

#include "sp/graphics/FontManager.h"

namespace sp { namespace graphics { namespace ui {

	using namespace maths;

	Button::Button(const String& label, const maths::Rectangle& bounds, const ActionHandler& handler)
		: Widget(bounds), m_Label(label), m_ActionHandler(handler), m_State(ButtonState::UNPRESSED)
	{
		m_Font = FontManager::Get(24);
	}

	bool Button::OnMousePressed(events::MousePressedEvent& e)
	{
		m_State = ButtonState::PRESSED;
		return true;
	}

	bool Button::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		if (m_State == ButtonState::PRESSED)
			OnAction();

		m_State = ButtonState::UNPRESSED;
		return true;
	}

	bool Button::OnMouseMoved(events::MouseMovedEvent& e)
	{
		// TODO: Remove these hard coded mouse maths throughout the engine
		vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
		if (m_State == ButtonState::PRESSED && !m_Bounds.Contains(mouse))
  			m_State = ButtonState::UNPRESSED;

		return false;
	}

	void Button::OnAction()
	{
		m_ActionHandler();
	}

	void Button::OnUpdate()
	{
	}

	void Button::OnRender(Renderer2D& renderer)
	{
		renderer.DrawRect(m_Bounds);
		renderer.FillRect(m_Bounds, m_State == ButtonState::PRESSED ? 0xcfbbbbbb : 0xcf5f5f5f);
		renderer.DrawString(m_Label, m_Bounds.position + vec2(0.2f, 0.7f), *m_Font);
	}

} } }