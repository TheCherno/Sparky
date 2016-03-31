#pragma once

#include "sp/Common.h"
#include "../maths/maths.h"
#include "API/Texture2D.h"

namespace sp { namespace graphics {

	struct SP_API Mask
	{
		API::Texture* texture;
		maths::mat4 transform;

		Mask(API::Texture2D* texture, const maths::mat4& transform = maths::mat4::Identity())
			: texture(texture), transform(transform)
		{
			this->transform = maths::mat4::Scale(maths::vec3((float)texture->GetWidth() / (float)texture->GetHeight(), 1.0f, 1.0f));
		}

	};

} }