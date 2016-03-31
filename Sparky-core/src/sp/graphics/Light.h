#pragma once

#include "sp/Types.h"
#include "sp/maths/maths.h"

namespace sp { namespace graphics {

	__declspec(align(16)) struct SP_API Light
	{
		maths::vec4 color;
		maths::vec3 position;
		float p0;
		maths::vec3 direction;
		float p1;
		maths::vec3 lightVector;
		float intensity;

		Light(const maths::vec3& direction, float intensity = 1.0f, const maths::vec4& color = maths::vec4(1.0f));
	};

} }