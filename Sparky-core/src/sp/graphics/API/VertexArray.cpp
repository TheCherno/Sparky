#include "sp/sp.h"
#include "VertexArray.h"

#include "sp/platform/opengl/GLVertexArray.h"
#include "sp/platform/directx/DXVertexArray.h"

#include "sp/graphics/API/Context.h"
#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	VertexArray* VertexArray::Create()
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLVertexArray();
			case RenderAPI::DIRECT3D:	return spnew D3DVertexArray();
		}
		return nullptr;
	}

} } }