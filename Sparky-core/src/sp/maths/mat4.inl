#pragma once
#include <sstream>

#include "Quaternion.h"

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

namespace sp { namespace maths {

	SP_INLINE mat4::mat4()
	{
		memset(elements, 0, 4 * 4 * sizeof(float));
	}

	SP_INLINE mat4::mat4(float diagonal)
	{
		memset(elements, 0, 4 * 4 * sizeof(float));
		elements[0 + 0 * 4] = diagonal;
		elements[1 + 1 * 4] = diagonal;
		elements[2 + 2 * 4] = diagonal;
		elements[3 + 3 * 4] = diagonal;
	}

	SP_INLINE mat4::mat4(float* elements)
	{
		memcpy(this->elements, elements, 4 * 4 * sizeof(float));
	}

	SP_INLINE mat4::mat4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3)
	{
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
		rows[3] = row3;
	}
	
	SP_INLINE mat4 mat4::Identity()
	{
		return mat4(1.0f);
	}

	SP_INLINE const vec4& mat4::GetRow(uint index) const
	{
		SP_ASSERT(index < 4);
		return rows[index];
	}

	SP_INLINE vec4& mat4::GetRow(uint index)
	{
		SP_ASSERT(index < 4);
		return rows[index];
	}

	SP_INLINE vec4 mat4::GetColumn(uint index) const
	{
		SP_ASSERT(index < 4);
		return vec4(elements[index + 0 * 4], elements[index + 1 * 4], elements[index + 2 * 4], elements[index + 3 * 4]);
	}

	SP_INLINE void mat4::SetRow(uint index, const vec4& row)
	{
		SP_ASSERT(index < 4);
		rows[index] = row;
	}

	SP_INLINE void mat4::SetColumn(uint index, const vec4& column)
	{
		SP_ASSERT(index < 4);
		elements[index + 0 * 4] = column.x;
		elements[index + 1 * 4] = column.y;
		elements[index + 2 * 4] = column.z;
		elements[index + 3 * 4] = column.w;
	}

	SP_INLINE mat4& mat4::Multiply(const mat4& other)
	{
#if 1
		mat4 result;
 		//float data[16];
		//memset(data, 0, 4 * 4 * sizeof(float));
 		for (int32 row = 0; row < 4; row++)
 		{
 			for (int32 col = 0; col < 4; col++)
 			{
 				for (int32 element = 0; element < 4; element++)
 				{
					result(row, col) += (*this)(row, element) * other(element, col);
 				}
 			}
 		}

		*this = result;
 		return *this;
#else
		float data[16];
		for (int32 row = 0; row < 4; row++)
		{
			for (int32 col = 0; col < 4; col++)
			{
				float sum = 0.0f;
				for (int32 e = 0; e < 4; e++)
				{
					sum += elements[e + row * 4] * other.elements[col + e * 4];
				}
				data[col + row * 4] = sum;
			}
		}
		memcpy(elements, data, 4 * 4 * sizeof(float));
		return *this;
#endif
	}

	SP_INLINE vec3 mat4::Multiply(const vec3& other) const
	{
		return other.Multiply(*this);
	}

	SP_INLINE vec4 mat4::Multiply(const vec4& other) const
	{
		return other.Multiply(*this);
	}

	SP_INLINE mat4 operator*(mat4 left, const mat4& right)
	{
		return left.Multiply(right);
	}

	SP_INLINE mat4& mat4::operator*=(const mat4& other)
	{
		return Multiply(other);
	}

	SP_INLINE vec3 operator*(const mat4& left, const vec3& right)
	{
		return left.Multiply(right);
	}

	SP_INLINE vec4 operator*(const mat4& left, const vec4& right)
	{
		return left.Multiply(right);
	}

	SP_INLINE mat4& mat4::Invert()
	{
		float temp[16];

		temp[0] = elements[5] * elements[10] * elements[15] -
			elements[5] * elements[11] * elements[14] -
			elements[9] * elements[6] * elements[15] +
			elements[9] * elements[7] * elements[14] +
			elements[13] * elements[6] * elements[11] -
			elements[13] * elements[7] * elements[10];

		temp[4] = -elements[4] * elements[10] * elements[15] +
			elements[4] * elements[11] * elements[14] +
			elements[8] * elements[6] * elements[15] -
			elements[8] * elements[7] * elements[14] -
			elements[12] * elements[6] * elements[11] +
			elements[12] * elements[7] * elements[10];

		temp[8] = elements[4] * elements[9] * elements[15] -
			elements[4] * elements[11] * elements[13] -
			elements[8] * elements[5] * elements[15] +
			elements[8] * elements[7] * elements[13] +
			elements[12] * elements[5] * elements[11] -
			elements[12] * elements[7] * elements[9];

		temp[12] = -elements[4] * elements[9] * elements[14] +
			elements[4] * elements[10] * elements[13] +
			elements[8] * elements[5] * elements[14] -
			elements[8] * elements[6] * elements[13] -
			elements[12] * elements[5] * elements[10] +
			elements[12] * elements[6] * elements[9];

		temp[1] = -elements[1] * elements[10] * elements[15] +
			elements[1] * elements[11] * elements[14] +
			elements[9] * elements[2] * elements[15] -
			elements[9] * elements[3] * elements[14] -
			elements[13] * elements[2] * elements[11] +
			elements[13] * elements[3] * elements[10];

		temp[5] = elements[0] * elements[10] * elements[15] -
			elements[0] * elements[11] * elements[14] -
			elements[8] * elements[2] * elements[15] +
			elements[8] * elements[3] * elements[14] +
			elements[12] * elements[2] * elements[11] -
			elements[12] * elements[3] * elements[10];

		temp[9] = -elements[0] * elements[9] * elements[15] +
			elements[0] * elements[11] * elements[13] +
			elements[8] * elements[1] * elements[15] -
			elements[8] * elements[3] * elements[13] -
			elements[12] * elements[1] * elements[11] +
			elements[12] * elements[3] * elements[9];

		temp[13] = elements[0] * elements[9] * elements[14] -
			elements[0] * elements[10] * elements[13] -
			elements[8] * elements[1] * elements[14] +
			elements[8] * elements[2] * elements[13] +
			elements[12] * elements[1] * elements[10] -
			elements[12] * elements[2] * elements[9];

		temp[2] = elements[1] * elements[6] * elements[15] -
			elements[1] * elements[7] * elements[14] -
			elements[5] * elements[2] * elements[15] +
			elements[5] * elements[3] * elements[14] +
			elements[13] * elements[2] * elements[7] -
			elements[13] * elements[3] * elements[6];

		temp[6] = -elements[0] * elements[6] * elements[15] +
			elements[0] * elements[7] * elements[14] +
			elements[4] * elements[2] * elements[15] -
			elements[4] * elements[3] * elements[14] -
			elements[12] * elements[2] * elements[7] +
			elements[12] * elements[3] * elements[6];

		temp[10] = elements[0] * elements[5] * elements[15] -
			elements[0] * elements[7] * elements[13] -
			elements[4] * elements[1] * elements[15] +
			elements[4] * elements[3] * elements[13] +
			elements[12] * elements[1] * elements[7] -
			elements[12] * elements[3] * elements[5];

		temp[14] = -elements[0] * elements[5] * elements[14] +
			elements[0] * elements[6] * elements[13] +
			elements[4] * elements[1] * elements[14] -
			elements[4] * elements[2] * elements[13] -
			elements[12] * elements[1] * elements[6] +
			elements[12] * elements[2] * elements[5];

		temp[3] = -elements[1] * elements[6] * elements[11] +
			elements[1] * elements[7] * elements[10] +
			elements[5] * elements[2] * elements[11] -
			elements[5] * elements[3] * elements[10] -
			elements[9] * elements[2] * elements[7] +
			elements[9] * elements[3] * elements[6];

		temp[7] = elements[0] * elements[6] * elements[11] -
			elements[0] * elements[7] * elements[10] -
			elements[4] * elements[2] * elements[11] +
			elements[4] * elements[3] * elements[10] +
			elements[8] * elements[2] * elements[7] -
			elements[8] * elements[3] * elements[6];

		temp[11] = -elements[0] * elements[5] * elements[11] +
			elements[0] * elements[7] * elements[9] +
			elements[4] * elements[1] * elements[11] -
			elements[4] * elements[3] * elements[9] -
			elements[8] * elements[1] * elements[7] +
			elements[8] * elements[3] * elements[5];

		temp[15] = elements[0] * elements[5] * elements[10] -
			elements[0] * elements[6] * elements[9] -
			elements[4] * elements[1] * elements[10] +
			elements[4] * elements[2] * elements[9] +
			elements[8] * elements[1] * elements[6] -
			elements[8] * elements[2] * elements[5];

		float determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
		determinant = 1.0f / determinant;

		for (int32 i = 0; i < 4 * 4; i++)
			elements[i] = temp[i] * determinant;

		return *this;
	}

	SP_INLINE mat4 mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		mat4 result(1.0f);

		result.elements[0 + 0 * 4] = 2.0f / (right - left);

		result.elements[1 + 1 * 4] = 2.0f / (top - bottom);

		result.elements[2 + 2 * 4] = 2.0f / (near - far);

		result.elements[3 + 0 * 4] = (left + right) / (left - right);
		result.elements[3 + 1 * 4] = (bottom + top) / (bottom - top);
		result.elements[3 + 2 * 4] = (far + near) / (far - near);

		return Transpose(result);
	}

	SP_INLINE mat4 mat4::Perspective(float fov, float aspectRatio, float near, float far)
	{
		mat4 result(1.0f);

		float q = 1.0f / tan(toRadians(0.5f * fov));
		float a = q / aspectRatio;

		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);

		result.elements[0 + 0 * 4] = a;
		result.elements[1 + 1 * 4] = q;
		result.elements[2 + 2 * 4] = b;
		result.elements[2 + 3 * 4] = -1.0f;
		result.elements[3 + 2 * 4] = c;

		return Transpose(result);
	}

	SP_INLINE mat4 mat4::LookAt(const vec3& camera, const vec3& target, const vec3& up)
	{
#if 0
		mat4 result = Identity();
		vec3 f = (camera - target).Normalize();
		vec3 s = f.Cross(up.Normalize());
		vec3 u = f.Cross(s);

		float tx = -camera.Dot(s);
		float ty = -camera.Dot(u);
		float tz = -camera.Dot(f);

#if 1
		result.elements[0 + 0 * 4] = s.x;
		result.elements[0 + 1 * 4] = s.y;
		result.elements[0 + 2 * 4] = s.z;

		result.elements[1 + 0 * 4] = u.x;
		result.elements[1 + 1 * 4] = u.y;
		result.elements[1 + 2 * 4] = u.z;

		result.elements[2 + 0 * 4] = f.x;
		result.elements[2 + 1 * 4] = f.y;
		result.elements[2 + 2 * 4] = f.z;

		result.elements[3 + 0 * 4] = tx;
		result.elements[3 + 1 * 4] = ty;
		result.elements[3 + 2 * 4] = tz;
#else
		result.elements[0 + 0 * 4] = s.x;
		result.elements[1 + 0 * 4] = s.y;
		result.elements[2 + 0 * 4] = s.z;

		result.elements[0 + 1 * 4] = u.x;
		result.elements[1 + 1 * 4] = u.y;
		result.elements[2 + 1 * 4] = u.z;

		result.elements[0 + 2 * 4] = f.x;
		result.elements[1 + 2 * 4] = f.y;
		result.elements[2 + 2 * 4] = f.z;

		result.elements[0 + 3 * 4] = tx;
		result.elements[1 + 3 * 4] = ty;
		result.elements[2 + 3 * 4] = tz;
#endif
#endif

		mat4 result = Identity();
		vec3 z = (target - camera).Normalize();
		vec3 x = up.Cross(z).Normalize();
		vec3 y = z.Cross(x);

		float tx = -x.Dot(camera);
		float ty = -y.Dot(camera);
		float tz = -z.Dot(camera);

		result.SetColumn(0, vec4(x, tx));
		result.SetColumn(1, vec4(y, ty));
		result.SetColumn(2, vec4(z, tz));

		return result;
	}

	SP_INLINE mat4 mat4::Translate(const vec3& translation)
	{
		mat4 result(1.0f);

		result.elements[3 + 0 * 4] = translation.x;
		result.elements[3 + 1 * 4] = translation.y;
		result.elements[3 + 2 * 4] = translation.z;

		return Transpose(result);
	}

	SP_INLINE mat4 mat4::Rotate(float angle, const vec3& axis)
	{
		mat4 result(1.0f);

		float r = toRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;
		
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.elements[0 + 0 * 4] = x * omc + c;
		result.elements[0 + 1 * 4] = y * x * omc + z * s;
		result.elements[0 + 2 * 4] = x * z * omc - y * s;

		result.elements[1 + 0 * 4] = x * y * omc - z * s;
		result.elements[1 + 1 * 4] = y * omc + c;
		result.elements[1 + 2 * 4] = y * z * omc + x * s;

		result.elements[2 + 0 * 4] = x * z * omc + y * s;
		result.elements[2 + 1 * 4] = y * z * omc - x * s;
		result.elements[2 + 2 * 4] = z * omc + c;
		
		return Transpose(result);
	}

	SP_INLINE mat4 mat4::Rotate(const Quaternion& quat)
	{
		mat4 result = Identity();

		float qx, qy, qz, qw, qx2, qy2, qz2, qxqx2, qyqy2, qzqz2, qxqy2, qyqz2, qzqw2, qxqz2, qyqw2, qxqw2;
		qx = quat.x;
		qy = quat.y;
		qz = quat.z;
		qw = quat.w;
		qx2 = (qx + qx);
		qy2 = (qy + qy);
		qz2 = (qz + qz);
		qxqx2 = (qx * qx2);
		qxqy2 = (qx * qy2);
		qxqz2 = (qx * qz2);
		qxqw2 = (qw * qx2);
		qyqy2 = (qy * qy2);
		qyqz2 = (qy * qz2);
		qyqw2 = (qw * qy2);
		qzqz2 = (qz * qz2);
		qzqw2 = (qw * qz2);

		result.rows[0] = vec4(((1.0f - qyqy2) - qzqz2), (qxqy2 - qzqw2), (qxqz2 + qyqw2), 0.0f);
		result.rows[1] = vec4((qxqy2 + qzqw2), ((1.0f - qxqx2) - qzqz2), (qyqz2 - qxqw2), 0.0f);
		result.rows[2] = vec4((qxqz2 - qyqw2), (qyqz2 + qxqw2), ((1.0f - qxqx2) - qyqy2), 0.0f);
		return Transpose(result);
	}

	SP_INLINE mat4 mat4::Scale(const vec3& scale)
	{
		mat4 result(1.0f);

		result.elements[0 + 0 * 4] = scale.x;
		result.elements[1 + 1 * 4] = scale.y;
		result.elements[2 + 2 * 4] = scale.z;

		return result;
	}

	SP_INLINE mat4 mat4::Invert(const mat4& matrix)
	{
		mat4 result = matrix;
		return result.Invert();
	}

	SP_INLINE mat4 mat4::Transpose(const mat4& matrix)
	{
		return mat4(
			vec4(matrix.rows[0].x, matrix.rows[1].x, matrix.rows[2].x, matrix.rows[3].x),
			vec4(matrix.rows[0].y, matrix.rows[1].y, matrix.rows[2].y, matrix.rows[3].y),
			vec4(matrix.rows[0].z, matrix.rows[1].z, matrix.rows[2].z, matrix.rows[3].z),
			vec4(matrix.rows[0].w, matrix.rows[1].w, matrix.rows[2].w, matrix.rows[3].w)
			);
	}

} }