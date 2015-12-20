#include "sp/sp.h"
#include "ShaderFactory.h"

namespace sp { namespace graphics { namespace ShaderFactory {

#if defined(SP_PLATFORM_WINDOWS)

	static const char* s_DefaultShader =
#include "default/Default.shader"
		;

	static const char* s_SimpleShader =
#include "default/Simple.shader"
		;

	static const char* s_BasicLightShader =
#include "default/BasicLight.shader"
		;

#else
#error TODO: GLES shaders!
#endif

	Shader* DefaultShader()
	{
		return Shader::FromSource("Default Shader", s_DefaultShader);
	}

	Shader* SimpleShader()
	{
		return Shader::FromSource("Simple Shader", s_SimpleShader);
	}

	Shader* BasicLightShader()
	{
		return Shader::FromSource("Basic Light Shader", s_BasicLightShader);
	}

} } }