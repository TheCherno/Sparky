#pragma once

#if defined(SP_PLATFORM_WIN32)
#include <GL/glew.h>
#elif defined(SP_PLATFORM_ANDROID)
#include <GLES2/gl2.h>

#define GL_CLAMP			GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_BORDER	GL_CLAMP_TO_EDGE
#endif
