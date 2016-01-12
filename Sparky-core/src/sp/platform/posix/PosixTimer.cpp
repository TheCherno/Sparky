// UNTESTED
#include "sp/sp.h"
#include "sp/utils/Timer.h"

#include <chrono>

namespace sp {

	typedef std::chrono::high_resolution_clock HighResolutionClock;
	typedef std::chrono::duration<float, std::milli> milliseconds_type;

	static std::chrono::time_point<HighResolutionClock> s_Start;

	Timer::Timer()
	{
		Reset();
	}

	void Timer::Reset()
	{
		s_Start = HighResolutionClock::now();
	}

	float Timer::Elapsed()
	{
		return std::chrono::duration_cast<milliseconds_type>(HighResolutionClock::now() - s_Start).count() / 1000.0f;
	}
}