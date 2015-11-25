#pragma once

#include "vec3.h"
#include "vec4.h"
#include "maths_func.h"

namespace sp { namespace maths {

	struct mat4
	{
		union
		{
			float elements[4 * 4];
			vec4 columns[4];
		};

		mat4();
		mat4(float diagonal);

		vec4 getColumn(int index)
		{
			index *= 4;
			return vec4(elements[index], elements[index + 1], elements[index + 2], elements[index + 3]);
		}

		static mat4 Identity();

		mat4& Multiply(const mat4& other);
		friend mat4 operator*(mat4 left, const mat4& right);
		mat4& operator*=(const mat4& other);

		vec3 Multiply(const vec3& other) const;
		friend vec3 operator*(const mat4& left, const vec3& right);

		vec4 Multiply(const vec4& other) const;
		friend vec4 operator*(const mat4& left, const vec4& right);

		mat4& Invert();

		static mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
		static mat4 Perspective(float fov, float aspectRatio, float near, float far);

		static mat4 Translate(const vec3& translation);
		static mat4 Rotate(float angle, const vec3& axis);
		static mat4 Scale(const vec3& scale);
		static mat4 Invert(const mat4& matrix);
	};

} }