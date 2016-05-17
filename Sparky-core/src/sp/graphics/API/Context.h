#pragma once

#include "sp/Common.h"
#include "sp/app/Window.h"

namespace sp { namespace graphics { namespace API {

	enum class SP_API RenderAPI
	{
		NONE, OPENGL, DIRECT3D
	};

	class SP_API Context
	{
	protected:
		static Context* s_Context;
		static RenderAPI s_RenderAPI;
	public:
		static void Create(WindowProperties properties, void* deviceContext);

		static RenderAPI GetRenderAPI() { return s_RenderAPI; }
		static void SetRenderAPI(RenderAPI api) { s_RenderAPI = api; }
		static void SetVsync(const uint& syncInterval);
	};

} } }