#pragma once

#include "Event.h"
#include "../maths/vec2.h"

namespace sp { namespace events {

	class MouseButtonEvent : public Event
	{
	protected:
		int m_Button;
		maths::vec2 m_Position;
	protected:
		MouseButtonEvent(int button, float x, float y, Event::Type type);
	public:
		inline const int GetButton() const { return m_Button; }
		inline const float GetX() const { return m_Position.x; }
		inline const float GetY() const { return m_Position.y; }
		inline const maths::vec2& GetPosition() const { return m_Position; }
	};

	class MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(int button, float x, float y);
	};

	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(int button, float x, float y);
	};

	class MouseMovedEvent : public Event
	{
	private:
		maths::vec2 m_Position;
		bool m_Dragged;
	public:
		MouseMovedEvent(float x, float y, bool dragged);

		inline const float GetX() const { return m_Position.x; }
		inline const float GetY() const { return m_Position.y; }
		inline const maths::vec2& GetPosition() const { return m_Position; }
		inline const bool IsDragged() const { return m_Dragged; }
	};

} }