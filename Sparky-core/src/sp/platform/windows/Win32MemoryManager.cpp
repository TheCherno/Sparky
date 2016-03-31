#include "sp/sp.h"
#include "sp/system/MemoryManager.h"

#include <Windows.h>

namespace sp { namespace internal {

	SystemMemoryInfo MemoryManager::GetSystemInfo()
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&status);

		SystemMemoryInfo result =
		{
			(int64)status.ullAvailPhys,
			(int64)status.ullTotalPhys,

			(int64)status.ullAvailVirtual,
			(int64)status.ullTotalVirtual
		};
		return result;
	}


} }