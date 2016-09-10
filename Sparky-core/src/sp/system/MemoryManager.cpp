#include "sp/sp.h"
#include "MemoryManager.h"

#include "sp/utils/Log.h"
#include "Memory.h"

namespace sp { namespace internal {

	MemoryManager* MemoryManager::s_Instance = nullptr;

	MemoryManager::MemoryManager()
	{
	}

	void MemoryManager::Init()
	{
	}

	void MemoryManager::Shutdown()
	{
		spdel s_Instance;
	}

	MemoryManager* MemoryManager::Get()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = (MemoryManager*)malloc(sizeof(MemoryManager));
			s_Instance = new(s_Instance) MemoryManager();
		}
		return s_Instance;
	}

	String MemoryManager::BytesToString(int64 bytes)
	{
		static const float gb = 1024 * 1024 * 1024;
		static const float mb = 1024 * 1024;
		static const float kb = 1024;

		String result;
		if (bytes > gb)
			result = StringFormat::Float(bytes / gb) + " GB";
		else if (bytes > mb)
			result = StringFormat::Float(bytes / mb) + " MB";
		else if (bytes > kb)
			result = StringFormat::Float(bytes / kb) + " KB";
		else
			result = StringFormat::Float((float)bytes) + " bytes";

		return result;
	}

	void SystemMemoryInfo::Log()
	{
		String apm, tpm, avm, tvm;

		uint gb = 1024 * 1024 * 1024;
		uint mb = 1024 * 1024;
		uint kb = 1024;

		apm = MemoryManager::BytesToString(availablePhysicalMemory);
		tpm = MemoryManager::BytesToString(totalPhysicalMemory);
		avm = MemoryManager::BytesToString(availableVirtualMemory);
		tvm = MemoryManager::BytesToString(totalVirtualMemory);

		SP_INFO("");
		SP_INFO("Memory Info:");
		SP_INFO("\tPhysical Memory (Avail/Total): ", apm, " / ", tpm);
		SP_INFO("\tVirtual Memory  (Avail/Total): ", avm, " / ", tvm);
		SP_INFO("");
	}

} }