#include "Quaternion.h"

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

	const Quaternion Quaternion::Identity()
	{
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Quaternion & Quaternion::operator =(const Quaternion & Quaternion)
	{
		x = Quaternion.x;
		y = Quaternion.y;
		z = Quaternion.z;
		w = Quaternion.w;
		return *this;
	}

	Quaternion & Quaternion::SetXYZ(const vec3 & vec)
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

	Quaternion & Quaternion::SetElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	float Quaternion::GetElem(int idx) const
	{
		return *(&x + idx);
	}

	float Quaternion::operator [](int idx) const
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

	float Norm(const Quaternion & Quaternion)
	{
		float result;
		result = (Quaternion.x * Quaternion.x);
		result = (result + (Quaternion.y * Quaternion.y));
		result = (result + (Quaternion.z * Quaternion.z));
		result = (result + (Quaternion.w * Quaternion.w));
		return result;
	}

	float Length(const Quaternion & Quaternion)
	{
		return sqrt(Norm(Quaternion));
	}

	const Quaternion Normalize(const Quaternion & Quaternion)
	{
		float lenSqr, lenInv;
		lenSqr = Norm(Quaternion);
		lenInv = rsqrt(lenSqr);
		return Quaternion * lenInv;
	}

	const Quaternion NormalizeEst(const Quaternion & Quaternion)
	{
		float lenSqr, lenInv;
		lenSqr = Norm(Quaternion);
		lenInv = rsqrt(lenSqr);
		return Quaternion * lenInv;
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
		return Quaternion(
			(((w * quat.x) + (x * quat.w)) + (y * quat.z)) - (z * quat.y),
			(((w * quat.y) + (y * quat.w)) + (z * quat.x)) - (x * quat.z),
			(((w * quat.z) + (z * quat.w)) + (x * quat.y)) - (y * quat.x),
			(((w * quat.w) - (x * quat.x)) - (y * quat.y)) - (z * quat.z)
			);
	}

	vec3 Quaternion::Rotate(const Quaternion & quat, const vec3 & vec)
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