#pragma once

#include "Shader.h"

namespace sp { namespace graphics { namespace ShaderFactory {

	Shader* DefaultShader();
	Shader* SimpleShader();
	Shader* BasicLightShader();

} } }