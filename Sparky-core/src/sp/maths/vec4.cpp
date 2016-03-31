#include "sp/sp.h"
#include "vec4.h"

#include "mat4.h"

namespace sp { namespace maths {

	vec4::vec4(float scalar)
		: x(scalar), y(scalar), z(scalar), w(scalar)
	{
	}

	vec4::vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	vec4::vec4(const vec3& xyz, float w)
		: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
	{
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

	vec4 vec4::Multiply(const mat4& transform) const
	{
		return vec4(
			transform.rows[0].x * x + transform.rows[0].y * y + transform.rows[0].z * z + transform.rows[0].w * w,
			transform.rows[1].x * x + transform.rows[1].y * y + transform.rows[1].z * z + transform.rows[1].w * w,
			transform.rows[2].x * x + transform.rows[2].y * y + transform.rows[2].z * z + transform.rows[2].w * w,
			transform.rows[3].x * x + transform.rows[3].y * y + transform.rows[3].z * z + transform.rows[3].w * w
			);
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

	float vec4::Dot(const vec4& other)
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	std::ostream& operator<<(std::ostream& stream, const vec4& vector)
	{
		stream << "vec4: (" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
		return stream;
	}


} }