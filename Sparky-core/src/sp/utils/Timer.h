#pragma once

#include "sp/Common.h"

namespace sp {

	class SP_API Timer
	{
	public:
		// Creates and starts timer
		Timer();
		// Resets and restarts timer
		void Reset();
		// Returns time in milliseconds
		float Elapsed();
	};

}