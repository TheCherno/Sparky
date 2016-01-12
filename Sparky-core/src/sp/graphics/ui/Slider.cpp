#include "sp/sp.h"
#include "Slider.h"

namespace sp { namespace graphics { namespace ui {

	using namespace maths;

	Slider::Slider(const maths::Rectangle& bounds, bool vertical)
		: Widget(bounds), m_Value(0.0f), m_State(SliderState::UNPRESSED), m_HeadOffset(0.0f),
		m_Callback(&Slider::NoCallback), m_Vertical(vertical)
	{
		float size = vertical ? bounds.width : bounds.height;
		m_HeadBounds = Rectangle(bounds.x, bounds.y, size, size);
	}

	Slider::Slider(const maths::Rectangle& bounds, float value, const ValueChangedCallback& callback, bool vertical)
		: Widget(bounds), m_Value(value), m_State(SliderState::UNPRESSED), m_HeadOffset(0.0f),
		m_Callback(callback), m_Vertical(vertical)
	{
		float size = vertical ? bounds.width : bounds.height;
		m_HeadBounds = Rectangle(bounds.x, bounds.y, size, size);
	}

	bool Slider::OnMousePressed(events::MousePressedEvent& e)
	{
		// TODO: Remove these hard coded mouse maths throughout the engine
		vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
		if (m_HeadBounds.Contains(mouse))
		{
			m_State = SliderState::PRESSEDHEAD;
			m_HeadOffset = m_Vertical ? (mouse.y - m_HeadBounds.y) : (mouse.x - m_HeadBounds.x);
		}
		else
		{
			m_State = SliderState::PRESSED;
		}
		return true;
	}

	bool Slider::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		m_State = SliderState::UNPRESSED;
		return true;
	}

	bool Slider::OnMouseMoved(events::MouseMovedEvent& e)
	{
		// TODO: Remove these hard coded mouse maths throughout the engine
		vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
		if (m_State == SliderState::PRESSEDHEAD)
		{
			if (m_Vertical)
				SetValue((mouse.y - m_Bounds.y - m_HeadOffset) / (m_Bounds.size.y - m_HeadBounds.size.y));
			else
				SetValue((mouse.x - m_Bounds.x - m_HeadOffset) / (m_Bounds.size.x - m_HeadBounds.size.x));
		}

		return false;
	}

	void Slider::OnUpdate()
	{
		if (!Input::IsMouseButtonPressed(SP_MOUSE_LEFT))
			m_State = SliderState::UNPRESSED;

		if (m_Vertical)
		{
			float bounds = m_Bounds.size.y - m_HeadBounds.size.y;
			m_HeadBounds.y = m_Bounds.y + bounds * m_Value;
			m_HeadBounds.y = clamp(m_HeadBounds.y, m_Bounds.y, m_Bounds.y + m_Bounds.size.y - m_HeadBounds.size.y);
		}
		else
		{
			float bounds = m_Bounds.size.x - m_HeadBounds.size.x;
			m_HeadBounds.x = m_Bounds.x + bounds * m_Value;
			m_HeadBounds.x = clamp(m_HeadBounds.x, m_Bounds.x, m_Bounds.x + m_Bounds.size.x - m_HeadBounds.size.x);
		}
	}

	void Slider::OnRender(Renderer2D& renderer)
	{
		renderer.FillRect(m_Bounds, 0xcf7f7f7f);
		renderer.DrawRect(m_Bounds);

		renderer.FillRect(m_HeadBounds, 0xcfbfbfbf);
		renderer.DrawRect(m_HeadBounds);

		vec2 offset = m_Vertical ? vec2(0, m_Bounds.size.y / 2.0f) : vec2(m_Bounds.size.x / 2.0f, 0);
		renderer.DrawLine(m_Bounds.Center() - offset, m_Bounds.Center() + offset);
	}

	void Slider::SetValue(float value)
	{
		value = clamp(value, 0.0f, 1.0f);
		m_Value = value;
		m_Callback(value);
	}

} } }