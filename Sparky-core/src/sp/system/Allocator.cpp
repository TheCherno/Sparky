#include "sp/sp.h"
#include "Allocator.h"

#include <malloc.h>

#include "sp/utils/Log.h"
#include "MemoryManager.h"

namespace sp {

#if defined(SP_PLATFORM_WIN32)
	#define SP_MEMORY_ALIGNMENT 16
	#define SP_ALLOC(size)	_aligned_malloc(size, SP_MEMORY_ALIGNMENT)
	#define SP_FREE(block)	_aligned_free(block);
#elif defined(SP_PLATFORM_ANDROID)
	#define SP_ALLOC(size)	malloc(size)
	#define SP_FREE(block)	free(block);
#endif


	void* Allocator::Allocate(size_t size)
	{
		SP_ASSERT(size < 1024 * 1024 * 1024, "");

		sp::internal::MemoryManager::Get()->m_MemoryStats.totalAllocated += size;
		sp::internal::MemoryManager::Get()->m_MemoryStats.currentUsed += size;
		sp::internal::MemoryManager::Get()->m_MemoryStats.totalAllocations++;

		size_t actualSize = size + sizeof(size_t);
		byte* result = (byte*)SP_ALLOC(actualSize);
		memset(result, 0, actualSize);
		memcpy(result, &size, sizeof(size_t));
		result += sizeof(size_t);
		return result;
	}

	void* Allocator::AllocateDebug(size_t size, const char* file, uint line)
	{
		SP_ASSERT(size < 1024 * 1024 * 1024, "");

		if (size > 1024 * 1024)
			SP_WARN("Large allocation (>1mb) at ", file, ":", line);

		return Allocate(size);
	}

	void Allocator::Free(void* block)
	{
		byte* memory = ((byte*)block) - sizeof(size_t);
		size_t size = *(size_t*)memory;
		sp::internal::MemoryManager::Get()->m_MemoryStats.totalFreed += size;
		sp::internal::MemoryManager::Get()->m_MemoryStats.currentUsed -= size;
		SP_FREE(memory);
	}

	void Allocator::FreeDebug(void* block, const char* file, uint line)
	{
		Free(block);
	}

}