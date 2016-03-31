#include "sp/sp.h"
#include "LightSetup.h"

namespace sp { namespace graphics {

	Light::Light(const maths::vec3& direction, float intensity, const maths::vec4& color)
		: direction(direction), intensity(intensity), color(color), p0(0.0f), p1(0.0f), position(maths::vec3(0.0f)), lightVector(maths::vec3(0.0f))
	{
	}

} }