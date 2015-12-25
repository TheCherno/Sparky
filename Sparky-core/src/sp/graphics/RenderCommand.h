#pragma once

#include "sp/Common.h"
#include "Mesh.h"

namespace sp { namespace graphics {

	struct SP_API RendererUniform
	{
		String uniform;
		byte* value;
	};

	struct SP_API RenderCommand
	{
		Mesh* mesh;
		std::vector<RendererUniform> uniforms;
	};

} }
