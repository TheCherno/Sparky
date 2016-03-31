#pragma once

#include "Framebuffer.h"

namespace sp { namespace graphics {

	class SP_API Framebuffer2D : public Framebuffer
	{
	public:
		virtual void SetClearColor(const maths::vec4& color) = 0;
	public:
		static Framebuffer2D* Create(uint width, uint height);
	};

} }