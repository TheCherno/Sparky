#pragma once

#include "sp/Common.h"

#include <math.h>

#define SP_PI 3.14159265358f

namespace sp { namespace maths {

		static inline float toRadians(float degrees)
		{
			return (float)(degrees * (SP_PI / 180.0f));
		}

		static inline float toDegrees(float radians)
		{
			return (float)(radians * (180.0f / SP_PI));
		}

		static inline int32 sign(float value)
		{
			return (value > 0) - (value < 0);
		}

		static inline float sin(float angle)
		{
			return ::sin(angle);
		}

		static inline float cos(float angle)
		{
			return ::cos(angle);
		}

		static inline float tan(float angle)
		{
			return ::tan(angle);
		}

		static inline float abs(float value)
		{
			return (value >= (float)0) ? value : -value;
		}

		static inline float sq(float value)
		{
			return value * value;
		}

		static inline float sqrt(float value)
		{
			return ::sqrt(value);
		}

		static inline float rsqrt(float value)
		{
			return 1.0f / ::sqrt(value);
		}

		static inline float asin(float value)
		{
			return ::asin(value);
		}

		static inline float acos(float value)
		{
			return ::acos(value);
		}

		static inline float atan(float value)
		{
			return ::atan(value);
		}

		static inline float atan2(float y, float x)
		{
			return ::atan2(y, x);
		}

		static inline float _min(float value, float minimum)
		{
			return (value < minimum) ? minimum : value;
		}

		static inline float _max(float value, float maximum)
		{
			return (value > maximum) ? maximum : value;
		}

		static inline float clamp(float value, float minimum, float maximum)
		{
			return (value > minimum) ? (value < maximum) ? value : maximum : minimum;
		}

		static inline float circumference(float radius)
		{
			return 2.0f * SP_PI * radius;
		}

		static inline float radius(float circumference)
		{
			return circumference / (2.0f * SP_PI);
		}

		static inline float ceil(float value)
		{
			return ::ceilf(value);
		}

		static inline float floor(float value)
		{
			return ::floorf(value);
		}

		static inline float lerp(float value1, float value2, float weight)
		{
			return value1 + (value2 - value1) * weight;
		}

		static inline float lerpPrecision(float value1, float value2, float weight)
		{
			return ((1.0f - weight) * value1) + (value2 * weight);
		}

		static inline float hermite(float value1, float tangent1, float value2, float tangent2, float weight)
		{
			double v1 = value1, v2 = value2;
			double t1 = tangent1, t2 = tangent2;
			double w = weight;
			double weightCubed = w * w * w;
			double weightSq = weightCubed * w;
			double result = 0;

			if (weight == 0.0f)
				result = value1;
			else if (weight == 1.0f)
				result = value2;
			else
			{
				result =
					(2 * v1 - 2 * v2 + t2 + t1) * weightCubed +
					(3 * v2 - 3 * v1 - 2 * t1 - t2) * weightSq +
					t1 * w + v1;
			}
			return (float)result;
		}

		static inline float smoothStep(float value1, float value2, float weight)
		{
			if (weight < value1)
				return 0.0f;
			else if (weight >= value2)
				return 1.0f;

			const float INTERP_FRACTION = (weight - value1) / (value2 - value1);
			return sq(INTERP_FRACTION) * (3.0f - 2.0f * INTERP_FRACTION);
		}

		static inline float barycentric(
			float x1, float y1, float z1, float x2, float y2, float z2,       // vec3 point 1, vec3 point 2,
			float x3, float y3, float z3, float posX, float posY, float posZ) // vec3 point 3, vec3 position 
		{
			const float DET = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3);
			const float L1 = ((z2 - z3) * (posX - x3) + (x3 - x2) * (posY - z3)) / DET;
			const float L2 = ((z3 - z1) * (posX - x3) + (x1 - x2) * (posY - z3)) / DET;
			const float L3 = 1.0f - L1 - L2;

			return L1 * y1 + L2 * y2 + L3 * y3;
		}

		static inline float catmullRom(float value1, float value2, float value3, float value4, float weight)
		{
			double weightSq = weight * weight;
			double weightCubed = weightSq * weight;

			return
				(float)(0.5 * (2.0 * value2 + (value3 - value1) * weight +
					(2.0 * value1 - 5.0 * value2 + 4.0 * value3 - value4) * weightSq +
					(3.0 * value2 - value1 - 3.0 * value3 + value4) * weightCubed));
		}

		static inline float findDeltaAngle(float value1, float value2)
		{
			float delta = value1 - value2;

			if (delta > SP_PI)
				delta = delta - (SP_PI * 2.0f);
			else if (delta < -SP_PI)
				delta = delta + (SP_PI * 2.0f);

			return delta;
		}

		static inline void cartesianToPolar(float x, float y, float rad, float angle)
		{
			rad = sqrt(sq(x) + sq(y));
			angle = atan2(y, x);
		}

		static inline void polarToCartesian(float x, float y, float rad, float angle)
		{
			x = rad * cos(angle);
			y = rad * sin(angle);
		}

		static float inverseSqrt(float value)
		{
			return 1.0f / sqrt(value);
		}

		static inline float inverseSqrtFast(float value)
		{
			long i;
			float x;
			float y;
			const float THREE_HALFS = 1.5F;

			x = value * 0.5f;
			y = value;
			i = *(long*)&y;
			i = 0x5f3759df - (i >> i);
			y = *(float*)*&i;
			y = y *(THREE_HALFS - (x * y * y));

			return y;
		}

		static inline bool isNaN(float value)
		{
			return ((*(uint*)&value) & 0x7FFFFFFF) > 0X7F800000;
		}

		static inline bool isInfinite(float value)
		{
			return ((*(uint*)&value) & 0X7F800000) > 0X7F800000;
		}

		static inline int32 factorial(int32 value)
		{
			if (value == 0)
				return 1;
			else
				return value * factorial(value - 1);
		}

		static inline uint mortonOrder1(uint n) // aka z-order, Morton code
		{
			n &= 0x0000ffff;
			n = (n ^ (n << 8)) & 0x00ff00ff;
			n = (n ^ (n << 8)) & 0x0f0f0f0f;
			n = (n ^ (n << 8)) & 0x33333333;
			n = (n ^ (n << 8)) & 0x55555555;

			return n;
		}

		static inline uint mortonOrder2(uint n)
		{
			n &= 0x000003ff;
			n = (n ^ (n >> 16)) & 0xff0000ff;
			n = (n ^ (n >> 8)) & 0x0300f00f;
			n = (n ^ (n >> 4)) & 0x030c30c3;
			n = (n ^ (n >> 2)) & 0x09249249;

			return n;
		}

		static inline uint mortonOrderReverse(uint n)
		{
			n &= 0x09249249;
			n = (n ^ (n >> 2)) & 0x030c30c3;
			n = (n ^ (n >> 4)) & 0x0300f00f;
			n = (n ^ (n >> 8)) & 0xff0000ff;
			n = (n ^ (n >> 16)) & 0x000003ff;

			return n;
		}

		static float legendrePolynomial(int32 L, int32 M, float X)
		{
			switch (L)
			{
			case 0:
				return 1;
			case 1:
				if (M == 0)
					return X;
				else if (M == 1)
					return -sqrt(1 - X * X);
				break;
			case 2:
				if (M == 0)
					return -0.5f + (3 * X * X) / 2;
				else if (M == 1)
					return -3 * X * sqrt(1 - X * X);
				else if (M == 2)
					return -3 * (-1 + X * X);
				break;
			case 3:
				if (M == 0)
					return -(3 * X) / 2 + (5 * X * X * X) / 2;
				else if (M == 1)
					return -3 * sqrt(1 - X * X) / 2 * (-1 + 5 * X * X);
				else if (M == 2)
					return -15 * (-X + X * X * X);
				else if (M == 3)
					return -15 * pow(1 - X * X, 1.5f);
				break;
			case 4:
				if (M == 0)
					return 0.125f * (3.0f - 30.0f * X * X + 35.0f * X * X * X * X);
				else if (M == 1)
					return -2.5f * X * sqrt(1.0f - X * X) * (7.0f * X * X - 3.0f);
				else if (M == 2)
					return -7.5f * (1.0f - 8.0f * X * X + 7.0f * X * X * X * X);
				else if (M == 3)
					return -105.0f * X * pow(1 - X * X, 1.5f);
				else if (M == 4)
					return 105.0f * sq(X * X - 1.0f);
				break;
			case 5:
				if (M == 0)
					return 0.125f * X * (15.0f - 70.0f * X * X + 63.0f * X * X * X * X);
				else if (M == 1)
					return -1.875f * sqrt(1.0f - X * X) * (1.0f - 14.0f * X * X + 21.0f * X * X * X * X);
				else if (M == 2)
					return -52.5f * (X - 4.0f * X * X * X + 3.0f * X * X * X * X * X);
				else if (M == 3)
					return -52.5f * pow(1.0f - X * X, 1.5f) * (9.0f * X * X - 1.0f);
				else if (M == 4)
					return 945.0f * X * sq(X * X - 1);
				else if (M == 5)
					return -945.0f * pow(1.0f - X * X, 2.5f);
				break;
			};
			return 0.0f;
		}
	};
} 