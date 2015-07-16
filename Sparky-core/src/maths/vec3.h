#pragma once

#include <iostream>
#include <sparky_types.h>

namespace sparky { namespace maths {

	struct vec2;

	struct vec3
	{
		float x, y, z;

		vec3();
		vec3(float x, float y, float z);
		vec3(const vec2& other);

		vec3& Add(const vec3& other);
		vec3& Subtract(const vec3& other);
		vec3& Multiply(const vec3& other);
		vec3& Divide(const vec3& other);

		friend vec3 operator+(vec3 left, const vec3& right);
		friend vec3 operator-(vec3 left, const vec3& right);
		friend vec3 operator*(vec3 left, const vec3& right);
		friend vec3 operator/(vec3 left, const vec3& right);

		bool operator==(const vec3& other);
		bool operator!=(const vec3& other);

		vec3& operator+=(const vec3& other);
		vec3& operator-=(const vec3& other);
		vec3& operator*=(const vec3& other);
		vec3& operator/=(const vec3& other);

		float Distance(const vec3& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);
	};

} }