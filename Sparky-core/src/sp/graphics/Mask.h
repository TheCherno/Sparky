#pragma once

#include "sp/Common.h"
#include "../maths/maths.h"
#include "Texture.h"

namespace sp { namespace graphics {

	struct SP_API Mask
	{
		Texture* texture;
		maths::mat4 transform;

		Mask(Texture* texture, const maths::mat4& transform = maths::mat4::Identity())
			: texture(texture), transform(transform)
		{
			this->transform = maths::mat4::Scale(maths::vec3((float)texture->GetWidth() / (float)texture->GetHeight(), 1.0f, 1.0f));
		}

	};

} }