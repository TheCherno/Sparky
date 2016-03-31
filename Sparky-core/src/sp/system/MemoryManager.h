#pragma once

#include "sp/String.h"

namespace sp {
	namespace internal {

		struct SP_API SystemMemoryInfo
		{
			int64 availablePhysicalMemory;
			int64 totalPhysicalMemory;

			int64 availableVirtualMemory;
			int64 totalVirtualMemory;

			void Log();
		};

		struct SP_API MemoryStats
		{
			int64 totalAllocated;
			int64 totalFreed;
			int64 currentUsed;
			int64 totalAllocations;

			MemoryStats()
				: totalAllocated(0), totalFreed(0), currentUsed(0), totalAllocations(0)
			{
			}
		};

		class SP_API MemoryManager
		{
		public:
			static MemoryManager* s_Instance;
		public:
			MemoryStats m_MemoryStats;
		public:
			MemoryManager();

			static void Init();
			static void Shutdown();

			static MemoryManager* Get();
			inline MemoryStats GetMemoryStats() const { return m_MemoryStats; }
		public:
			SystemMemoryInfo GetSystemInfo();
		public:
			static String BytesToString(int64 bytes);
		};

	}
}
