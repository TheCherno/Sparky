#include "sp/sp.h"
#include "VFS.h"

#include "Memory.h"
#include "FileSystem.h"
#include "sp/utils/Log.h"

namespace sp {

	VFS* VFS::s_Instance = nullptr;

	void VFS::Init()
	{
		s_Instance = spnew VFS();
	}

	void VFS::Shutdown()
	{
		spdel s_Instance;
	}
	
	void VFS::Mount(const String& virtualPath, const String& physicalPath)
	{
		SP_ASSERT(s_Instance);
		m_MountPoints[virtualPath].push_back(physicalPath);
	}

	void VFS::Unmount(const String& path)
	{
		SP_ASSERT(s_Instance);
		m_MountPoints[path].clear();
	}

	byte* VFS::ReadFile(const String& path)
	{
		SP_ASSERT(s_Instance);
		if (m_MountPoints[path].empty())
			return FileSystem::FileExists(path) ? FileSystem::ReadFile(path) : nullptr;

		for (const String& physicalPath : m_MountPoints[path])
		{
			if (FileSystem::FileExists(physicalPath))
				return FileSystem::ReadFile(physicalPath);
		}
	}

	String VFS::ReadTextFile(const String& path)
	{
		SP_ASSERT(s_Instance);
		if (m_MountPoints[path].empty())
			return FileSystem::FileExists(path) ? FileSystem::ReadTextFile(path) : String();

		for (const String& physicalPath : m_MountPoints[path])
		{
			if (FileSystem::FileExists(physicalPath))
				return FileSystem::ReadTextFile(physicalPath);
		}
	}

	bool VFS::WriteFile(const String& path, byte* buffer)
	{
		SP_ASSERT(s_Instance);
		if (m_MountPoints[path].empty())
			return FileSystem::WriteFile(path, buffer);

		const String& physicalPath = m_MountPoints[path].front();
		return FileSystem::WriteFile(physicalPath, buffer);
	}

	bool VFS::WriteTextFile(const String& path, const String& text)
	{
		SP_ASSERT(s_Instance);
		if (m_MountPoints[path].empty())
			return FileSystem::WriteTextFile(path, text);

		const String& physicalPath = m_MountPoints[path].front();
		return FileSystem::WriteTextFile(physicalPath, text);
	}

}