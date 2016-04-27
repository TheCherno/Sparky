#pragma once

#include "Framebuffer.h"
#include "Texture2D.h"

namespace sp { namespace graphics {

	class SP_API Framebuffer2D : public Framebuffer
	{
	public:
		virtual void SetClearColor(const maths::vec4& color) = 0;
		virtual API::Texture2D* GetTexture2D() const = 0;
	public:
		static Framebuffer2D* Create(uint width, uint height);
	};

} }