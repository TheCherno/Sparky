#pragma once

#include "Event.h"
#include "../maths/vec2.h"

namespace sp { namespace events {

	class SP_API MouseButtonEvent : public Event
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

		inline static int GetStaticType() { return (int)Event::Type::MOUSE_PRESSED | (int)Event::Type::MOUSE_RELEASED; }
	};

	class SP_API MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(int button, float x, float y);

		String ToString() const override;

		inline static Type GetStaticType() { return Event::Type::MOUSE_PRESSED; }
	};

	class SP_API MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(int button, float x, float y);

		inline static Type GetStaticType() { return Event::Type::MOUSE_RELEASED; }
	};

	class SP_API MouseMovedEvent : public Event
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

		inline static Type GetStaticType() { return Event::Type::MOUSE_MOVED; }
	};

} }