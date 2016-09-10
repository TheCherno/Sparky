#include "sp/sp.h"
#include "VertexArray.h"

#include "sp/platform/opengl/GLVertexArray.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXVertexArray.h"
#endif

#include "sp/graphics/API/Context.h"
#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	VertexArray* VertexArray::Create()
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLVertexArray();
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return spnew D3DVertexArray();
#endif
		}
		return nullptr;
	}

} } }