// UNTESTED
#include "sp/sp.h"
#include "sp/utils/Timer.h"

#include <chrono>

namespace sp {

	typedef std::chrono::high_resolution_clock HighResolutionClock;
	typedef std::chrono::duration<float, std::milli> milliseconds_type;

	struct Members
	{
		std::chrono::time_point<HighResolutionClock> m_Start;
	};


	Timer::Timer()
		: m_Members(new (m_Reserved) Members())
	{
		Reset();
	}

	void Timer::Reset()
	{
		m_Members->m_Start = HighResolutionClock::now();
	}

	float Timer::Elapsed()
	{
		return std::chrono::duration_cast<milliseconds_type>(HighResolutionClock::now() - m_Members->m_Start).count() / 1000.0f;
	}

	float Timer::ElapsedMillis()
	{
		return Elapsed() * 1000.0f;
	}
}