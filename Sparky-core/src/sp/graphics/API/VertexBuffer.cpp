#include "sp/sp.h"
#include "VertexBuffer.h"

#include "sp/platform/opengl/GLVertexBuffer.h"
#include "sp/platform/directx/DXVertexBuffer.h"

#include "sp/graphics/API/Context.h"

namespace sp { namespace graphics { namespace API {

	VertexBuffer* VertexBuffer::Create(BufferUsage usage)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLVertexBuffer(usage);
			case RenderAPI::DIRECT3D:	return spnew D3DVertexBuffer(usage);
		}
		return nullptr;
	}

} } }