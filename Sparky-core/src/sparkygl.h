#pragma once

#ifdef SPARKY_PLATFORM_WEB
	#define GLFW_INCLUDE_ES3
#else
	#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <utils/Log.h>

static void log_gl_call(const char* function, const char* file, int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		SPARKY_ERROR("[OpenGL Error] (", error, "): ", function, " ", file, ":", line);
	}
}

#ifdef SPARKY_DEBUG
	#define GLCall(x) glGetError();\
		x; \
		log_gl_call(#x, __FILE__, __LINE__);
#else
	#define GLCall(x) x
#endif