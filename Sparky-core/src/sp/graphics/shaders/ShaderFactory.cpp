#include "sp/sp.h"
#include "ShaderFactory.h"

#include "sp/graphics/API/Context.h"

namespace sp { namespace graphics { namespace ShaderFactory {

#if defined(SP_PLATFORM_WINDOWS)

	static const char* s_BatchRendererShaderGL =
#include "sp/platform/opengl/shaders/BatchRenderer.shader"
		;

	static const char* s_BatchRendererShaderD3D =
#include "sp/platform/directx/shaders/BatchRenderer.hlsl"
		;

	static const char* s_SimpleShader =
#include "default/Simple.shader"
		;

	static const char* s_BasicLightShader =
#include "default/BasicLight.shader"
		;

	static const char* s_GeometryPassShader =
#include "default/GeometryPass.shader"
		;

	static const char* s_DebugShaderGL =
#include "sp/platform/opengl/shaders/Debug.shader"
		;

	static const char* s_DebugShaderD3D =
#include "sp/platform/directx/shaders/Debug.hlsl"
		;

#else
	#error TODO: GLES shaders!
#endif

	API::Shader* BatchRendererShader()
	{
		switch (API::Context::GetRenderAPI())
		{
			case API::RenderAPI::OPENGL:	return API::Shader::CreateFromSource("BatchRenderer", s_BatchRendererShaderGL);
			case API::RenderAPI::DIRECT3D:	return API::Shader::CreateFromSource("BatchRenderer", s_BatchRendererShaderD3D);
		}
		return nullptr;
	}

	API::Shader* SimpleShader()
	{
		return API::Shader::CreateFromSource("Simple Shader", s_SimpleShader);
	}

	API::Shader* BasicLightShader()
	{
		return API::Shader::CreateFromSource("Basic Light Shader", s_BasicLightShader);
	}

	API::Shader* GeometryPassShader()
	{
		return API::Shader::CreateFromSource("Geometry Pass Shader", s_GeometryPassShader);
	}

	API::Shader* DebugShader()
	{
		switch (API::Context::GetRenderAPI())
		{
		case API::RenderAPI::OPENGL:	return API::Shader::CreateFromSource("Debug Shader", s_DebugShaderGL);
		case API::RenderAPI::DIRECT3D:	return API::Shader::CreateFromSource("Debug Shader", s_DebugShaderD3D);
		}
		return nullptr;
	}

} } }