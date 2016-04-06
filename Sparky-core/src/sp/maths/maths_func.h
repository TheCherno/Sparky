#pragma once

#include "sp/Common.h"

#include <math.h>

#define SP_PI 3.14159265358f

namespace sp { namespace maths {

	SP_API inline float toRadians(float degrees)
	{
		return (float)(degrees * (SP_PI / 180.0f));
	}

	SP_API inline float toDegrees(float radians)
	{
		return (float)(radians * (180.0f / SP_PI));
	}

	SP_API inline int32 sign(float value)
	{
		return (value > 0) - (value < 0);
	}

	SP_API inline float sin(float angle)
	{
		return ::sin(angle);
	}

	SP_API inline float cos(float angle)
	{
		return ::cos(angle);
	}

	SP_API inline float tan(float angle)
	{
		return ::tan(angle);
	}

	SP_API inline float sqrt(float value)
	{
		return ::sqrt(value);
	}

	SP_API inline float rsqrt(float value)
	{
		return 1.0f / ::sqrt(value);
	}

	SP_API inline float asin(float value)
	{
		return ::asin(value);
	}

	SP_API inline float acos(float value)
	{
		return ::acos(value);
	}

	SP_API inline float atan(float value)
	{
		return ::atan(value);
	}

	SP_API inline float atan2(float y, float x)
	{
		return ::atan2(y, x);
	}

	SP_API inline float _min(float value, float minimum)
	{
		return (value < minimum) ? minimum : value;
	}

	SP_API inline float _max(float value, float maximum)
	{
		return (value > maximum) ? maximum : value;
	}

	SP_API inline float clamp(float value, float minimum, float maximum)
	{
		return (value > minimum) ? (value < maximum) ? value : maximum : minimum;
	}

} }