#include "sp/sp.h"
#include "TextureDepth.h"

#include "sp/platform/opengl/GLTextureDepth.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXTextureDepth.h"
#endif

#include "sp/graphics/API/Context.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	TextureDepth* TextureDepth::Create(uint width, uint height)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTextureDepth(width, height);
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return spnew D3DTextureDepth(width, height);
#endif
		}
		return nullptr;
	}

} } }