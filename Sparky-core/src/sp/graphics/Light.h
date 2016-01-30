#pragma once

#include "sp/Types.h"
#include "sp/maths/maths.h"

namespace sp { namespace graphics {

	struct SP_API Light
	{
		maths::vec3 position;
		float attenuation;
		maths::vec4 color;
	};

} }