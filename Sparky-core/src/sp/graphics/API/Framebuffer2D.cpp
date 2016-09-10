#include "sp/sp.h"
#include "Framebuffer2D.h"

#include "Context.h"

#include "sp/platform/opengl/GLFramebuffer2D.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXFramebuffer2D.h"
#endif

#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	Framebuffer2D* Framebuffer2D::Create(uint width, uint height)
	{
		switch (API::Context::GetRenderAPI())
		{
			case API::RenderAPI::OPENGL:	return spnew GLFramebuffer2D(width, height);
#ifdef SP_PLATFORM_WIN32
			case API::RenderAPI::DIRECT3D:	return spnew D3DFramebuffer2D(width, height);
#endif
		}
		return nullptr;
	}

} }