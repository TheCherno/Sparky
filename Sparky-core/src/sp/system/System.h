#pragma once

#include "MemoryManager.h"

namespace sp { namespace internal {

	struct SystemInfo
	{
		SystemMemoryInfo memoryInfo;
		// GraphicsInfo graphicsInfo;
	};

	// Low-level system operations
	class System
	{
	private:
		static SystemInfo s_SystemInfo;
	public:
		static void Init();
		static void Shutdown();

		static SystemInfo GetSystemInfo();
	private:
		static void LogSystemInfo();
	};

} }