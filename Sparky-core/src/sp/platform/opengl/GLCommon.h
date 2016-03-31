#pragma once

#include <GL/glew.h>

namespace sp {
	extern GLenum GLCheckError();
	extern bool GLLogCall(const char* function, const char* file, int32 line);
}

#ifdef SP_DEBUG
	#define GLCall(x) sp::GLCheckError();\
		x; \
		if (!sp::GLLogCall(#x, __FILE__, __LINE__)) __debugbreak();
#else
	#define GLCall(x) x
#endif
