#pragma once

#include "mat4.h"

namespace sp { namespace maths {

	struct Quaternion
	{
		float x, y, z, w;

		Quaternion();
		Quaternion(const Quaternion& Quaternion);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const vec3& xyz, float w);
		Quaternion(const vec4& vec);
		Quaternion(float scalar);

		Quaternion& operator=(const Quaternion& quat);

		Quaternion& SetXYZ(const vec3& vec);
		const vec3 GetXYZ() const;

		Quaternion& SetElem(int idx, float value);
		float GetElem(int idx) const;
		vec3 GetAxis() const;

		const Quaternion operator+(const Quaternion& Quaternion) const;
		const Quaternion operator-(const Quaternion& Quaternion) const;
		const Quaternion operator*(const Quaternion& Quaternion) const;
		const Quaternion operator*(float scalar) const;
		const Quaternion operator/(float scalar) const;
		float operator[](int idx) const;

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
		bool operator==(const Quaternion & Quaternion) const;
		bool operator!=(const Quaternion & Quaternion) const;
		static const Quaternion Identity();

		static vec3 Rotate(const Quaternion & quat, const vec3 & vec);

		static const Quaternion Rotation(const vec3 & unitVec0, const vec3 & unitVec1);
		static const Quaternion Rotation(float radians, const vec3 & unitVec);

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