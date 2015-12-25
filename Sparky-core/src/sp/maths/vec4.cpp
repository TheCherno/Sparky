#include "sp/sp.h"
#include "vec4.h"

namespace sp { namespace maths {

	vec4::vec4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	vec4& vec4::Add(const vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	vec4& vec4::Subtract(const vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	vec4& vec4::Multiply(const vec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	vec4& vec4::Divide(const vec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}

	vec4 operator+(vec4 left, const vec4& right)
	{
		return left.Add(right);
	}

	vec4 operator-(vec4 left, const vec4& right)
	{
		return left.Subtract(right);
	}

	vec4 operator*(vec4 left, const vec4& right)
	{
		return left.Multiply(right);
	}

	vec4 operator/(vec4 left, const vec4& right)
	{
		return left.Divide(right);
	}

	vec4& vec4::operator+=(const vec4& other)
	{
		return Add(other);
	}

	vec4& vec4::operator-=(const vec4& other)
	{
		return Subtract(other);
	}

	vec4& vec4::operator*=(const vec4& other)
	{
		return Multiply(other);
	}

	vec4& vec4::operator/=(const vec4& other)
	{
		return Divide(other);
	}

	bool vec4::operator==(const vec4& other)
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool vec4::operator!=(const vec4& other)
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const vec4& vector)
	{
		stream << "vec4: (" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
		return stream;
	}


} }