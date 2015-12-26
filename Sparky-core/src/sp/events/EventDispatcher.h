#pragma once

#include "Events.h"
#include <functional>

namespace sp { namespace events {

	class EventDispatcher
	{
	private:
		Event& m_Event;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		void Dispatch(std::function<bool(T&)> func)
		{
			if ((int)m_Event.GetType() & (int)T::GetStaticType())
				m_Event.m_Handled = func(*(T*)&m_Event);
		}
	};

} }
