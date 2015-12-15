#pragma once

#include "Mesh.h"

namespace sp { namespace graphics {

	struct RendererUniform
	{
		String uniform;
		byte* value;
	};

	struct RenderCommand
	{
		Mesh* mesh;
		std::vector<RendererUniform> uniforms;
	};

} }
