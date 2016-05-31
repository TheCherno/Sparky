#include "sp/sp.h"
#include "Checkbox.h"

#include "sp/graphics/FontManager.h"

namespace sp { namespace graphics { namespace ui {

	using namespace maths;

	Checkbox::Checkbox(const String& label, const maths::Rectangle& bounds, const ActionHandler& handler)
		: Widget(bounds), m_Label(label), m_ActionHandler(handler), m_State(ButtonState::UNPRESSED)
	{
		m_Font = FontManager::Get(24);
	}

	bool Checkbox::OnMousePressed(events::MousePressedEvent& e)
	{
		return true;
	}

	bool Checkbox::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		if (m_State == ButtonState::PRESSED) m_State = ButtonState::UNPRESSED;
		else m_State = ButtonState::PRESSED;

		OnAction();
		return true;
	}

	bool Checkbox::OnMouseMoved(events::MouseMovedEvent& e)
	{
		return false;
	}

	void Checkbox::OnAction()
	{
		m_ActionHandler(m_State);
	}

	void Checkbox::OnUpdate()
	{
	}

	void Checkbox::OnRender(Renderer2D& renderer)
	{
		renderer.DrawRect(m_Bounds);
		renderer.FillRect(m_Bounds, 0xcf5f5f5f);
		renderer.DrawString(m_Label, m_Bounds.position + vec2(m_Bounds.width + 0.03f, m_Bounds.height / 2), *m_Font);
		if (m_State == ButtonState::PRESSED)
			renderer.DrawTexture(m_Bounds, API::Texture2D::CreateFromFile("res/tb.png", API::TextureLoadOptions(false, true)), Rectangle(0.0f, 0.0f, 1.0f, 1.0f));
	}

} } }