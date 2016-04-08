#include "sp/sp.h"
#include "Quaternion.h"

#include "mat4.h"

namespace sp { namespace maths {

#define _VECTORMATH_SLERP_TOL 0.999f

	Quaternion::Quaternion()
		: x(0), y(0), z(0), w(1)
	{
	}

	Quaternion::Quaternion(const Quaternion& Quaternion)
	{
		x = Quaternion.x;
		y = Quaternion.y;
		z = Quaternion.z;
		w = Quaternion.w;
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	Quaternion::Quaternion(const vec4& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	Quaternion::Quaternion(float scalar)
	{
		x = scalar;
		y = scalar;
		z = scalar;
		w = scalar;
	}

	Quaternion::Quaternion(const vec3& xyz, float w)
	{
		this->SetXYZ(xyz);
		this->w = w;
	}

	Quaternion Quaternion::Identity()
	{
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Quaternion Quaternion::FromEulerAngles(const vec3& angles)
	{
		Quaternion pitch(vec3(1.0, 0.0, 0.0), angles.x);
		Quaternion yaw(vec3(0.0, 1.0, 0.0), angles.y);
		Quaternion roll(vec3(0.0, 0.0, 1.0), angles.z);
		return pitch * yaw * roll;
	}

	Quaternion& Quaternion::operator =(const Quaternion & Quaternion)
	{
		x = Quaternion.x;
		y = Quaternion.y;
		z = Quaternion.z;
		w = Quaternion.w;
		return *this;
	}

	Quaternion& Quaternion::SetXYZ(const vec3 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	const vec3 Quaternion::GetXYZ() const
	{
		return vec3(x, y, z);
	}

	Quaternion& Quaternion::SetElem(int32 idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	float Quaternion::GetElem(int32 idx) const
	{
		return *(&x + idx);
	}

	float Quaternion::operator [](int32 idx) const
	{
		return *(&x + idx);
	}

	vec3 Quaternion::GetAxis() const
	{
		float x = 1.0f - w * w;
		if (x < 0.0000001f) // Divide by zero safety check
			return vec3::XAxis();

		float x2 = x * x;
		return GetXYZ() / x2;
	}

	vec3 Quaternion::ToEulerAngles() const
	{
		return vec3(atan2(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z),
			atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z),
			asin(2 * x * y + 2 * z * w));
	}

	const Quaternion Quaternion::operator+(const Quaternion& quaternion) const
	{
		return Quaternion(x + quaternion.x, y + quaternion.y, z + quaternion.z, w + quaternion.w);
	}

	const Quaternion Quaternion::operator-(const Quaternion& quaternion) const
	{
		return Quaternion(x - quaternion.x, y - quaternion.y, z - quaternion.z, w - quaternion.w);
	}

	const Quaternion Quaternion::operator*(float scalar) const
	{
		return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	const Quaternion Quaternion::operator/(float scalar) const
	{
		return Quaternion(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	const Quaternion Quaternion::operator-() const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	bool Quaternion::operator ==(const Quaternion & Quaternion) const
	{
		return (x == Quaternion.x) && (y == Quaternion.y) && (z == Quaternion.z) && (w == Quaternion.w);
	}

	bool Quaternion::operator !=(const Quaternion & Quaternion) const
	{
		return !(*this == Quaternion);
	}

	float Norm(const Quaternion& quaternion)
	{
		float result;
		result = (quaternion.x * quaternion.x);
		result = (result + (quaternion.y * quaternion.y));
		result = (result + (quaternion.z * quaternion.z));
		result = (result + (quaternion.w * quaternion.w));
		return result;
	}

	float Length(const Quaternion& quaternion)
	{
		return sqrt(Norm(quaternion));
	}

	const Quaternion Normalize(const Quaternion& quaternion)
	{
		float lenSqr, lenInv;
		lenSqr = Norm(quaternion);
		lenInv = rsqrt(lenSqr);
		return quaternion * lenInv;
	}

	const Quaternion NormalizeEst(const Quaternion& quaternion)
	{
		float lenSqr, lenInv;
		lenSqr = Norm(quaternion);
		lenInv = rsqrt(lenSqr);
		return quaternion * lenInv;
	}

	const Quaternion Quaternion::Rotation(const vec3& unitVec0, const vec3& unitVec1)
	{
		float cosHalfAngleX2, recipCosHalfAngleX2;
		cosHalfAngleX2 = sqrt((2.0f * (1.0f + unitVec0.Dot(unitVec1))));
		recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
		return Quaternion((unitVec0.Cross(unitVec1) * recipCosHalfAngleX2), (cosHalfAngleX2 * 0.5f));
	}

	const Quaternion Quaternion::Rotation(float radians, const vec3 & unitVec)
	{
		float angle = radians * 0.5f;
		return Quaternion((unitVec * sin(angle)), cos(angle));
	}

	const Quaternion Quaternion::operator*(const Quaternion& quat) const
	{
		return Normalize(Quaternion(
			(((w * quat.x) + (x * quat.w)) + (y * quat.z)) - (z * quat.y),
			(((w * quat.y) + (y * quat.w)) + (z * quat.x)) - (x * quat.z),
			(((w * quat.z) + (z * quat.w)) + (x * quat.y)) - (y * quat.x),
			(((w * quat.w) - (x * quat.x)) - (y * quat.y)) - (z * quat.z)
			));
	}

	vec3 Quaternion::Rotate(const Quaternion& quat, const vec3& vec)
	{
		float tmpX, tmpY, tmpZ, tmpW;
		tmpX = (((quat.w * vec.x) + (quat.y * vec.z)) - (quat.z * vec.y));
		tmpY = (((quat.w * vec.y) + (quat.z * vec.x)) - (quat.x * vec.z));
		tmpZ = (((quat.w * vec.z) + (quat.x * vec.y)) - (quat.y * vec.x));
		tmpW = (((quat.x * vec.x) + (quat.y * vec.y)) + (quat.z * vec.z));
		return vec3(
			((((tmpW * quat.x) + (tmpX * quat.w)) - (tmpY * quat.z)) + (tmpZ * quat.y)),
			((((tmpW * quat.y) + (tmpY * quat.w)) - (tmpZ * quat.x)) + (tmpX * quat.z)),
			((((tmpW * quat.z) + (tmpZ * quat.w)) - (tmpX * quat.y)) + (tmpY * quat.x))
			);
	}

	Quaternion Quaternion::Conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	const Quaternion Select(const Quaternion& quat0, const Quaternion& quat1, bool select1)
	{
		return Quaternion(select1 ? quat1.x : quat0.x, select1 ? quat1.y : quat0.y, select1 ? quat1.z : quat0.z, select1 ? quat1.w : quat0.w);
	}

	float Quaternion::Dot(const Quaternion& other) const
	{
		float result = (x * other.x);
		result = (result + (y * other.y));
		result = (result + (z * other.z));
		result = (result + (w * other.w));
		return result;
	}

} }