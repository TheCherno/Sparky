#pragma once

#include "Allocator.h"

#define spnew		new(__FILE__, __LINE__)
#define spdel		delete

#pragma warning(disable : 4595)

inline void* operator new(size_t size)
{
	return sp::Allocator::Allocate(size);
}

inline void* operator new(size_t size, const char* file, uint line)
{
	return sp::Allocator::AllocateDebug(size, file, line);
}

inline void* operator new[](size_t size)
{
	return sp::Allocator::Allocate(size);
}

inline void* operator new[](size_t size, const char* file, uint line)
{
	return sp::Allocator::AllocateDebug(size, file, line);
}

inline void operator delete(void* block)
{
	sp::Allocator::Free(block);
}

inline void operator delete(void* block, const char* file, uint line)
{
	sp::Allocator::FreeDebug(block, file, line);
}

inline void operator delete[](void* block)
{
	sp::Allocator::Free(block);
}

inline void operator delete[](void* block, const char* file, uint line)
{
	sp::Allocator::FreeDebug(block, file, line);
}

#pragma warning(default : 4595)
