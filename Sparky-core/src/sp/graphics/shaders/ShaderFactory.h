#pragma once

#include "sp/Common.h"
#include "Shader.h"

namespace sp { namespace graphics { namespace ShaderFactory {

	SP_API Shader* DefaultShader();
	SP_API Shader* SimpleShader();
	SP_API Shader* BasicLightShader();

} } }