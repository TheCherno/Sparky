#include "sp/sp.h"
#include "Context.h"

#include "sp/platform/opengl/GLContext.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXContext.h"
#endif

#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	Context* Context::s_Context = nullptr;
	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;

	void Context::Create(WindowProperties properties, void* deviceContext)
	{
		switch (GetRenderAPI())
		{
			case RenderAPI::OPENGL:		s_Context = spnew GLContext(properties, deviceContext); break;
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	s_Context = spnew D3DContext(properties, deviceContext); break;
#endif
		}
	}

} } }