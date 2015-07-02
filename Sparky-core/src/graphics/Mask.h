#pragma once

#include "texture.h"
#include "../maths/maths.h"

namespace sparky { namespace graphics {

	struct Mask
	{
		Texture* texture;
		maths::mat4 transform;

		Mask(Texture* texture, const maths::mat4& transform = maths::mat4::identity())
			: texture(texture), transform(transform)
		{
			this->transform = maths::mat4::scale(maths::vec3((float)texture->getWidth() / (float)texture->getHeight(), 1.0f, 1.0f));
		}

	};

} }