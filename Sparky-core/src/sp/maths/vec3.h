#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

#include "sp/Types.h"

namespace sp { namespace maths {

	struct vec2;
	struct vec4;
	struct mat4;

	struct SP_API vec3
	{
		float x, y, z;

		vec3();
		vec3(float scalar);
		vec3(float x, float y, float z);
		vec3(const vec2& other);
		vec3(float x, float y);
		vec3(const vec4& other);

		static vec3 Up();
		static vec3 Down();
		static vec3 Left();
		static vec3 Right();
		static vec3 Zero();

		static vec3 XAxis();
		static vec3 YAxis();
		static vec3 ZAxis();

		vec3& Add(const vec3& other);
		vec3& Subtract(const vec3& other);
		vec3& Multiply(const vec3& other);
		vec3& Divide(const vec3& other);

		vec3& Add(float other);
		vec3& Subtract(float other);
		vec3& Multiply(float other);
		vec3& Divide(float other);

		vec3 Multiply(const mat4& transform) const;

		friend vec3 operator+(vec3 left, const vec3& right);
		friend vec3 operator-(vec3 left, const vec3& right);
		friend vec3 operator*(vec3 left, const vec3& right);
		friend vec3 operator/(vec3 left, const vec3& right);

		friend vec3 operator+(vec3 left, float right);
		friend vec3 operator-(vec3 left, float right);
		friend vec3 operator*(vec3 left, float right);
		friend vec3 operator/(vec3 left, float right);

		bool operator==(const vec3& other) const;
		bool operator!=(const vec3& other) const;

		vec3& operator+=(const vec3& other);
		vec3& operator-=(const vec3& other);
		vec3& operator*=(const vec3& other);
		vec3& operator/=(const vec3& other);

		vec3& operator+=(float other);
		vec3& operator-=(float other);
		vec3& operator*=(float other);
		vec3& operator/=(float other);

		bool operator<(const vec3& other) const;
		bool operator<=(const vec3& other) const;
		bool operator>(const vec3& other) const;
		bool operator>=(const vec3& other) const;

		friend vec3 operator-(const vec3& vector);

		vec3 Cross(const vec3& other) const;
		float Dot(const vec3& other) const;

		float Magnitude() const;
		vec3 Normalize() const;
		float Distance(const vec3& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);
	};

} }