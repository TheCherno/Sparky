#include "sp/sp.h"
#include "Framebuffer2D.h"

#include "Context.h"

#include "sp/platform/opengl/GLFramebuffer2D.h"
#include "sp/platform/directx/DXFramebuffer2D.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	Framebuffer2D* Framebuffer2D::Create(uint width, uint height)
	{
		switch (API::Context::GetRenderAPI())
		{
			case API::RenderAPI::OPENGL:	return spnew GLFramebuffer2D(width, height);
			case API::RenderAPI::DIRECT3D:	return spnew D3DFramebuffer2D(width, height);
		}
		return nullptr;
	}

} }