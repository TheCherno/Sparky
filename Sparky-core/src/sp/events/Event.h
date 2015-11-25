#pragma once

namespace sp { namespace events {

	class Event
	{
	public:
		enum class Type
		{
			MOUSE_PRESSED, MOUSE_RELEASED, MOUSE_MOVED,
			KEY_PRESSED, KEY_RELEASED
		};
	protected:
		bool m_Handled;
		Type m_Type;
	protected:
		Event(Type type);
		inline Type GetType() const { return m_Type; }
	};

} }