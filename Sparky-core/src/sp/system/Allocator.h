#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

namespace sp {

	// TODO: Allocation Metrics
	class SP_API Allocator
	{
	public:
		static void* Allocate(size_t size);
		static void* AllocateDebug(size_t size, const char* file, uint line);

		static void Free(void* block);
		static void FreeDebug(void* block, const char* file, uint line);
	};

}