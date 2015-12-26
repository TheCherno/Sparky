#pragma once

#include "sp/Common.h"
#include "sp/String.h"

namespace sp { namespace events {

#undef MOUSE_PRESSED
#undef MOUSE_RELEASED
#undef MOUSE_MOVED
#undef KEY_PRESSED
#undef KEY_RELEASED

	class SP_API Event
	{
	private:
		friend class EventDispatcher;
	public:
		enum class Type
		{
			KEY_PRESSED		= BIT(0),
			KEY_RELEASED	= BIT(1),

			MOUSE_PRESSED	= BIT(2),
			MOUSE_RELEASED	= BIT(3),
			MOUSE_MOVED		= BIT(4)
		};
	protected:
		bool m_Handled;
		Type m_Type;
	protected:
		Event(Type type);
	public:
		inline Type GetType() const { return m_Type; }
		inline bool IsHandled() const { return m_Handled; }

		virtual String ToString() const;

		static String TypeToString(Type type);
	};

} }