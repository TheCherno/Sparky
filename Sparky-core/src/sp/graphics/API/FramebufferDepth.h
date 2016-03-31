#pragma once

#include "Framebuffer.h"

namespace sp { namespace graphics {

	class SP_API FramebufferDepth : public Framebuffer
	{
	public:
		static FramebufferDepth* Create(uint width, uint height);
	};

} }