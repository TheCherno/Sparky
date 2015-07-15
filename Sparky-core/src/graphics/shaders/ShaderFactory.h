#pragma once

#include "Shader.h"

namespace sparky { namespace graphics { namespace ShaderFactory {

	Shader* DefaultShader();
	Shader* SimpleShader();
	Shader* BasicLightShader();

} } }