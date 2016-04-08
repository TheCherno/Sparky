#pragma once

#include "sp/Common.h"
#include "sp/String.h"

#include "vec3.h"
#include "vec4.h"
#include "maths_func.h"
#include "sp/utils/Log.h"

namespace sp { namespace maths {

	struct Quaternion;

	struct SP_API mat4
	{
		union
		{
			// [row + col * 4]
			float elements[4 * 4];
			vec4 rows[4];
		};

		SP_INLINE mat4();
		SP_INLINE mat4(float diagonal);
		SP_INLINE mat4(float* elements);
		SP_INLINE mat4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3);

		SP_INLINE static mat4 Identity();

		float& operator ()(int r, int c) { return elements[r * 4 + c]; }
		float const& operator ()(int r, int c) const { return elements[r * 4 + c]; }

		SP_INLINE mat4& Multiply(const mat4& other);
		SP_INLINE friend SP_API mat4 operator*(mat4 left, const mat4& right);
		SP_INLINE mat4& operator*=(const mat4& other);

		SP_INLINE vec3 Multiply(const vec3& other) const;
		SP_INLINE friend SP_API vec3 operator*(const mat4& left, const vec3& right);

		SP_INLINE vec4 Multiply(const vec4& other) const;
		SP_INLINE friend SP_API vec4 operator*(const mat4& left, const vec4& right);

		SP_INLINE mat4& Invert();

		SP_INLINE const vec4& GetRow(uint index) const;
		SP_INLINE vec4& GetRow(uint index);
		SP_INLINE vec4 GetColumn(uint index) const;
		SP_INLINE void SetRow(uint index, const vec4& row);
		SP_INLINE void SetColumn(uint index, const vec4& column);

		SP_INLINE static mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
		SP_INLINE static mat4 Perspective(float fov, float aspectRatio, float near, float far);
		SP_INLINE static mat4 LookAt(const vec3& camera, const vec3& target, const vec3& up);

		SP_INLINE static mat4 Translate(const vec3& translation);
		SP_INLINE static mat4 Rotate(float angle, const vec3& axis);
		SP_INLINE static mat4 Rotate(const Quaternion& quat);
		SP_INLINE static mat4 Scale(const vec3& scale);
		SP_INLINE static mat4 Invert(const mat4& matrix);

		SP_INLINE static mat4 Transpose(const mat4& matrix);

		String ToString() const;
	};

} }

#include "mat4.inl"