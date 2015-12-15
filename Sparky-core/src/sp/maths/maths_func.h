#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace sp { namespace maths {

	inline float toRadians(float degrees)
	{
		return (float)(degrees * (M_PI / 180.0));
	}

	inline float toDegrees(float radians)
	{
		return (float)(radians * (180.0f / M_PI));
	}

	inline int sign(float value)
	{
		return (value > 0) - (value < 0);
	}

	inline float sin(float angle)
	{
		return ::sin(angle);
	}

	inline float cos(float angle)
	{
		return ::cos(angle);
	}

	inline float tan(float angle)
	{
		return ::tan(angle);
	}

	inline float sqrt(float value)
	{
		return ::sqrt(value);
	}

	inline float rsqrt(float value)
	{
		return 1.0f / ::sqrt(value);
	}

	inline float asin(float value)
	{
		return ::asin(value);
	}

	inline float acos(float value)
	{
		return ::acos(value);
	}

	inline float atan(float value)
	{
		return ::atan(value);
	}

	inline float atan2(float y, float x)
	{
		return ::atan2(y, x);
	}

} }