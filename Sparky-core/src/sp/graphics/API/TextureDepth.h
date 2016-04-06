#pragma once

#include "Texture.h"

namespace sp { namespace graphics { namespace API {

	class SP_API TextureDepth : public Texture
	{
	public:
		static TextureDepth* Create(uint width, uint height);

		virtual void Clear() = 0;
		virtual void BindForWriting() const = 0;
		virtual void UnbindForWriting() const = 0;
	};

} } }