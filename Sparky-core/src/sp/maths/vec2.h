#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/String.h"

#include "vec3.h"

namespace sp { namespace maths {

	struct SP_API vec2
	{
		float x, y;

		vec2();
		explicit vec2(float scalar);
		vec2(float x, float y);
		explicit vec2(const vec3& vector);

		vec2& Add(const vec2& other);
		vec2& Subtract(const vec2& other);
		vec2& Multiply(const vec2& other);
		vec2& Divide(const vec2& other);

		vec2& Add(float value);
		vec2& Subtract(float value);
		vec2& Multiply(float value);
		vec2& Divide(float value);

		friend vec2 operator+(vec2 left, const vec2& right);
		friend vec2 operator-(vec2 left, const vec2& right);
		friend vec2 operator*(vec2 left, const vec2& right);
		friend vec2 operator/(vec2 left, const vec2& right);

		friend vec2 operator+(vec2 left, float value);
		friend vec2 operator-(vec2 left, float value);
		friend vec2 operator*(vec2 left, float value);
		friend vec2 operator/(vec2 left, float value);

		bool operator==(const vec2& other) const;
		bool operator!=(const vec2& other) const;

		vec2& operator+=(const vec2& other);
		vec2& operator-=(const vec2& other);
		vec2& operator*=(const vec2& other);
		vec2& operator/=(const vec2& other);

		vec2& operator+=(float value);
		vec2& operator-=(float value);
		vec2& operator*=(float value);
		vec2& operator/=(float value);

		bool operator<(const vec2& other) const;
		bool operator<=(const vec2& other) const;
		bool operator>(const vec2& other) const;
		bool operator>=(const vec2& other) const;

		float Magnitude() const;
		vec2 Normalise() const;
		float Distance(const vec2& other) const;
		float Dot(const vec2& other) const;

		String ToString() const;

		friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);
	};

} }