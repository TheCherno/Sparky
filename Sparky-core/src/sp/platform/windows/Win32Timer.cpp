#include "sp/sp.h"
#include "sp/utils/Timer.h"

#include <Windows.h>

namespace sp {

	static LARGE_INTEGER s_Start;
	static double s_Frequency;

	Timer::Timer()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		s_Frequency = 1.0 / frequency.QuadPart;

		Reset();
	}

	void Timer::Reset()
	{
		QueryPerformanceCounter(&s_Start);
	}

	float Timer::Elapsed()
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		unsigned __int64 cycles = current.QuadPart - s_Start.QuadPart;
		return (float)(cycles * s_Frequency);
	}
}