#include "sp/sp.h"
#include "System.h"

#include "MemoryManager.h"

#include "sp/utils/Log.h"

namespace sp { namespace internal {

	SystemInfo System::s_SystemInfo;

	void System::Init()
	{
		SP_INFO("Initializing Sparky System...");
		MemoryManager::Init();

		s_SystemInfo.memoryInfo = MemoryManager::Get()->GetSystemInfo();
		LogSystemInfo();
	}

	void System::Shutdown()
	{
		SP_INFO("Shutting down Sparky System...");
	}

	SystemInfo System::GetSystemInfo()
	{
		return s_SystemInfo;
	}

	void System::LogSystemInfo()
	{
		SP_INFO("--------------------");
		SP_INFO(" System Information ");
		SP_INFO("--------------------");
		s_SystemInfo.memoryInfo.Log();
	}

} }