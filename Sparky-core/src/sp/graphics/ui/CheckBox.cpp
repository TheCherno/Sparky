#include "sp/sp.h"
#include "CheckBox.h"

#include "sp/graphics/FontManager.h"

namespace sp { namespace graphics { namespace ui {
		
	using namespace maths;

	CheckBox::CheckBox(const String& label, const maths::Rectangle& bounds, const ActionHandler& handler)
		: Widget(bounds), m_Label(label), m_ActionHandler(handler)
	{
		m_Font = FontManager::Get(24);
		m_State = CheckBoxState::UNCHECKED;
	}

	bool CheckBox::OnMousePressed(events::MousePressedEvent& e)
	{
		if (m_State == CheckBoxState::UNCHECKED)
			m_State = CheckBoxState::CHECKED;
		else
			m_State = CheckBoxState::UNCHECKED;
		return true;
	}

	bool CheckBox::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		if (m_State == CheckBoxState::CHECKED)
			OnAction();

		return true;
	}

	bool CheckBox::OnMouseMoved(events::MouseMovedEvent& e)
	{
		// TODO: Remove these hard coded mouse maths throughout the engine
		vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
		if (m_Bounds.Contains(mouse))
			m_Focused = true;
		else
			m_Focused = false;
		return false;
	}

	void CheckBox::OnAction()
	{
		m_ActionHandler();
	}

	void CheckBox::OnUpdate()
	{
	}

	void CheckBox::OnRender(Renderer2D& renderer)
	{
		renderer.DrawRect(m_Bounds);
		uint32 color = 0xcfbbbbbb + (m_Focused ? 0x00222222 : 0x00000000);
		renderer.FillRect(m_Bounds, color);

		uint32 colorBox = (m_State == CheckBoxState::CHECKED ? 0xcf00a010 : 0xcf5f5f5f);
		const Rectangle boxBounds(m_Bounds.position.x + 0.4f + m_Bounds.size.x - (m_Bounds.size.x / 5.0f), m_Bounds.position.y + (m_Bounds.size.y / 2 - 0.25f), 0.5f, 0.5f);
		renderer.FillRect(boxBounds, colorBox);

		renderer.DrawString(m_Label, m_Bounds.position + vec2(0.2f, 0.7f), *m_Font);
	}

} } }