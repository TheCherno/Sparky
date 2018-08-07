#pragma once

#include "Event.h"
#include "sp/maths/tvec2.h"

namespace sp { namespace events {

	class SP_API WindowResizeEvent : public Event
	{
	protected:
		maths::tvec2<uint> m_Size;
	public:
		WindowResizeEvent(uint width, uint height);

		inline const maths::tvec2<uint>& GetSize() const { return m_Size; }
		inline const uint GetWidth() const { return m_Size.x; }
		inline const uint GetHeight() const { return m_Size.y; }
	public:
		inline static Type GetStaticType() { return Event::Type::WINDOW_RESIZE; }
	};

	class SP_API WindowFocusEvent : public Event
	{
	protected:
		bool m_Focus;
	public:
		WindowFocusEvent(bool focus);

		inline const bool GetFocus() const { return m_Focus; }

	public:
		inline static Type GetStaticType() { return Event::Type::WINDOW_FOCUS; }
	};

	class SP_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent();

	public:
		inline static Type GetStaticType() { return Event::Type::WINDOW_CLOSE; }
	};

	class SP_API WindowActiveEvent : public Event
	{
	protected:
		bool m_Active;

	public:
		WindowActiveEvent(bool active);

		inline const bool IsActive() const { return m_Active; }

	public:
		inline static Type GetStaticType() { return Event::Type::WINDOW_ACTIVE; }

	};

	class SP_API WindowMoveEvent : public Event
	{
	protected:
		maths::tvec2<uint> m_Position;
	public:
		WindowMoveEvent(uint x, uint y);

		inline const maths::tvec2<uint>& GetPosition() const { return m_Position; }
		inline const uint GetX() const { return m_Position.x; }
		inline const uint GetY() const { return m_Position.y; }

	public:
		inline static Type GetStaticType() { return Event::Type::WINDOW_MOVE; }
	};

} }
