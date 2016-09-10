#include "sp/sp.h"
#include "FramebufferDepth.h"

#include "Context.h"

#include "sp/platform/opengl/GLFramebufferDepth.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXFramebufferDepth.h"
#endif

#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	FramebufferDepth* FramebufferDepth::Create(uint width, uint height)
	{
		switch (API::Context::GetRenderAPI())
		{
			case API::RenderAPI::OPENGL:	return spnew GLFramebufferDepth(width, height);
#ifdef SP_PLATFORM_WIN32
			case API::RenderAPI::DIRECT3D:	return spnew D3DFramebufferDepth(width, height);
#endif
		}
		return nullptr;
	}

	
} }