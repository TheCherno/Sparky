#include "sp/sp.h"
#include "Memory.h"

void* operator new(size_t size)
{
	return sp::Allocator::Allocate(size);
}

void* operator new(size_t size, const char* file, uint line)
{
	return sp::Allocator::AllocateDebug(size, file, line);
}

void* operator new[](size_t size)
{
	return sp::Allocator::Allocate(size);
}

void* operator new[](size_t size, const char* file, uint line)
{
	return sp::Allocator::AllocateDebug(size, file, line);
}

void operator delete(void* block)
{
	sp::Allocator::Free(block);
}

void operator delete(void* block, const char* file, uint line)
{
	sp::Allocator::FreeDebug(block, file, line);
}

void operator delete[](void* block)
{
	sp::Allocator::Free(block);
}

void operator delete[](void* block, const char* file, uint line)
{
	sp::Allocator::FreeDebug(block, file, line);
}
