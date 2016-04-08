#include "sp/sp.h"

#undef NOGDI
#include <Windows.h>
#include <GL/glew.h>

#include "GLContext.h"
#include "sp/utils/Log.h"

namespace sp { namespace graphics { namespace API {

	static HDC hDc;

	GLContext::GLContext(WindowProperties, void* deviceContext)
	{
		hDc = GetDC((HWND)deviceContext);
		HGLRC hrc = wglCreateContext(hDc);
		SP_ASSERT(hrc, "Failed creating OpenGL context!");
		SP_ASSERT(wglMakeCurrent(hDc, hrc), "Failed setting OpenGL context!");
		SP_ASSERT(glewInit() == GLEW_OK, "Could not initialize GLEW!");
	}

	void GLContext::Present()
	{
		SwapBuffers(hDc);
	}

} } }