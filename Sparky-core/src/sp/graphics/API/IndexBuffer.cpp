#include "sp/sp.h"
#include "IndexBuffer.h"

#include "sp/platform/opengl/GLIndexBuffer.h"
#include "sp/platform/directx/DXIndexBuffer.h"

#include "sp/graphics/API/Context.h"

namespace sp { namespace graphics { namespace API {

	IndexBuffer* IndexBuffer::Create(uint16* data, uint count)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLIndexBuffer(data, count);
			case RenderAPI::DIRECT3D:	return spnew D3DIndexBuffer(data, count);
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLIndexBuffer(data, count);
			case RenderAPI::DIRECT3D:	return spnew D3DIndexBuffer(data, count);
		}
		return nullptr;
	}
	
} } }