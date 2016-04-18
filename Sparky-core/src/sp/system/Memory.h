#pragma once

#include "Allocator.h"

#define spnew		new(__FILE__, __LINE__)
#define spdel		delete

void* operator new(size_t size);
SP_API void* operator new(size_t size, const char* file, uint line);
void* operator new[](size_t size);
SP_API void* operator new[](size_t size, const char* file, uint line);
void operator delete(void* block);
SP_API void operator delete(void* block, const char* file, uint line);
void operator delete[](void* block);
SP_API void operator delete[](void* block, const char* file, uint line);
