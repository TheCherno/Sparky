#pragma once

#include "sp/Common.h"

namespace sp { namespace graphics {

	class Renderer3D;

	class SP_API IRenderable
	{
	public:
		virtual void Render(Renderer3D& renderer) = 0;
	};

} }
