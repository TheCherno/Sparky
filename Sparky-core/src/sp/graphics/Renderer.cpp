#include "sp/sp.h"
#include "Renderer.h"

#include "sp/platform/opengl/GLRenderer.h"
#include "sp/platform/directx/DXRenderer.h"

#include "API/Context.h"
#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	Renderer* Renderer::s_Instance = nullptr;

	void Renderer::Init()
	{
		switch (API::Context::GetRenderAPI())
		{
		case API::RenderAPI::OPENGL:	s_Instance = spnew GLRenderer(); break;
		case API::RenderAPI::DIRECT3D:	s_Instance = spnew D3DRenderer(); break;
		}
		s_Instance->InitInternal();
	}

} }