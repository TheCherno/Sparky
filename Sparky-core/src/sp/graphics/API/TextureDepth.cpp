#include "sp/sp.h"
#include "TextureDepth.h"

#include "sp/platform/opengl/GLTextureDepth.h"
#include "sp/platform/directx/DXTextureDepth.h"

#include "sp/graphics/API/Context.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	TextureDepth* TextureDepth::Create(uint width, uint height)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTextureDepth(width, height);
			case RenderAPI::DIRECT3D:	return spnew D3DTextureDepth(width, height);
		}
		return nullptr;
	}

} } }