#pragma once

#include "sp/Common.h"

#include "mat4.h"

namespace sp { namespace maths {

	struct SP_API Quaternion
	{
		float x, y, z, w;

		Quaternion();
		Quaternion(const Quaternion& quaternion);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const vec3& xyz, float w);
		explicit Quaternion(const vec4& vec);
		Quaternion(float scalar);

		Quaternion& operator=(const Quaternion& quat);

		Quaternion& SetXYZ(const vec3& vec);
		const vec3 GetXYZ() const;

		Quaternion& SetElem(int32 idx, float value);
		float GetElem(int32 idx) const;
		vec3 GetAxis() const;
		vec3 ToEulerAngles() const;

		const Quaternion operator+(const Quaternion& Quaternion) const;
		const Quaternion operator-(const Quaternion& Quaternion) const;
		const Quaternion operator*(const Quaternion& Quaternion) const;
		const Quaternion operator*(float scalar) const;
		const Quaternion operator/(float scalar) const;
		float operator[](int32 idx) const;

		Quaternion& operator+=(const Quaternion& Quaternion)
		{
			*this = *this + Quaternion;
			return *this;
		}

		Quaternion& operator-=(const Quaternion& Quaternion)
		{
			*this = *this - Quaternion;
			return *this;
		}

		Quaternion& operator*=(const Quaternion& Quaternion)
		{
			*this = *this * Quaternion;
			return *this;
		}

		Quaternion& operator*=(float scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		Quaternion& operator/=(float scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		const Quaternion operator-() const;
		bool operator==(const Quaternion& quaternion) const;
		bool operator!=(const Quaternion& quaternion) const;
		static Quaternion Identity();
		static Quaternion FromEulerAngles(const vec3& angles);

		static vec3 Rotate(const Quaternion & quat, const vec3 & vec);

		static const Quaternion Rotation(const vec3& unitVec0, const vec3& unitVec1);
		static const Quaternion Rotation(float radians, const vec3& unitVec);

		static const Quaternion RotationX(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(sin(angle), 0.0f, 0.0f, cos(angle));
		}

		static const Quaternion RotationY(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(0.0f, sin(angle), 0.0f, cos(angle));
		}

		static const Quaternion RotationZ(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(0.0f, 0.0f, sin(angle), cos(angle));
		}

		float Dot(const Quaternion& other) const;
		Quaternion Conjugate() const;

	};

} }