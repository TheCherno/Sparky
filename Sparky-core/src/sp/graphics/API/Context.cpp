#include "sp/sp.h"
#include "Context.h"

#include "sp/platform/opengl/GLContext.h"
#include "sp/platform/directx/DXContext.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	Context* Context::s_Context = nullptr;
	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;

	void Context::Create(WindowProperties properties, void* deviceContext)
	{
		switch (GetRenderAPI())
		{
			case RenderAPI::OPENGL:		s_Context = spnew GLContext(properties, deviceContext); break;
			case RenderAPI::DIRECT3D:	s_Context = spnew D3DContext(properties, deviceContext); break;
		}
	}

	void Context::SetVsync(const uint& syncInterval)
	{
		switch ( s_RenderAPI )
		{
			case RenderAPI::OPENGL:		GLContext::SetVsync(syncInterval); break;
			case RenderAPI::DIRECT3D:	D3DContext::SetVsync(syncInterval); break;
		}
	}

} } }