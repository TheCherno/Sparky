#include "sp/sp.h"
#include "vec3.h"
#include "vec2.h"

namespace sp { namespace maths {

	vec3::vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3::vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec3::vec3(const vec2& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = 0.0f;
	}

	vec3::vec3(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->z = 0.0f;
	}

	vec3 vec3::Up()
	{
		return vec3(0.0f, 1.0f, 0.0f);
	}

	vec3 vec3::Down()
	{
		return vec3(0.0f, -1.0f, 0.0f);
	}

	vec3 vec3::Left()
	{
		return vec3(-1.0f, 0.0f, 0.0f);
	}

	vec3 vec3::Right()
	{
		return vec3(1.0f, 1.0f, 0.0f);
	}

	vec3 vec3::Zero()
	{
		return vec3(0.0f, 0.0f, 0.0f);
	}

	vec3 vec3::XAxis()
	{
		return vec3(1.0f, 0.0f, 0.0f);
	}

	vec3 vec3::YAxis()
	{
		return vec3(0.0f, 1.0f, 0.0f);
	}

	vec3 vec3::ZAxis()
	{
		return vec3(0.0f, 0.0f, 1.0f);
	}

	vec3& vec3::Add(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	vec3& vec3::Subtract(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	vec3& vec3::Multiply(const vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	vec3& vec3::Divide(const vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}

	vec3& vec3::Add(float other)
	{
		x += other;
		y += other;
		z += other;

		return *this;
	}

	vec3& vec3::Subtract(float other)
	{
		x -= other;
		y -= other;
		z -= other;

		return *this;
	}

	vec3& vec3::Multiply(float other)
	{
		x *= other;
		y *= other;
		z *= other;

		return *this;
	}

	vec3& vec3::Divide(float other)
	{
		x /= other;
		y /= other;
		z /= other;

		return *this;
	}

	vec3 operator+(vec3 left, const vec3& right)
	{
		return left.Add(right);
	}

	vec3 operator-(vec3 left, const vec3& right)
	{
		return left.Subtract(right);
	}

	vec3 operator*(vec3 left, const vec3& right)
	{
		return left.Multiply(right);
	}

	vec3 operator/(vec3 left, const vec3& right)
	{
		return left.Divide(right);
	}

	vec3 operator+(vec3 left, float right)
	{
		return left.Add(right);
	}

	vec3 operator-(vec3 left, float right)
	{
		return left.Subtract(right);
	}

	vec3 operator*(vec3 left, float right)
	{
		return left.Multiply(right);
	}

	vec3 operator/(vec3 left, float right)
	{
		return left.Divide(right);
	}

	vec3& vec3::operator+=(const vec3& other)
	{
		return Add(other);
	}

	vec3& vec3::operator-=(const vec3& other)
	{
		return Subtract(other);
	}

	vec3& vec3::operator*=(const vec3& other)
	{
		return Multiply(other);
	}

	vec3& vec3::operator/=(const vec3& other)
	{
		return Divide(other);
	}

	vec3& vec3::operator+=(float other)
	{
		return Add(other);
	}

	vec3& vec3::operator-=(float other)
	{
		return Subtract(other);
	}

	vec3& vec3::operator*=(float other)
	{
		return Multiply(other);
	}

	vec3& vec3::operator/=(float other)
	{
		return Divide(other);
	}

	bool vec3::operator<(const vec3& other) const
	{
		return x < other.x && y < other.y && z < other.z;
	}

	bool vec3::operator<=(const vec3& other) const
	{
		return x <= other.x && y <= other.y && z <= other.z;
	}

	bool vec3::operator>(const vec3& other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	bool vec3::operator>=(const vec3& other) const
	{
		return x >= other.x && y >= other.y && z >= other.z;
	}

	bool vec3::operator==(const vec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool vec3::operator!=(const vec3& other) const
	{
		return !(*this == other);
	}

	vec3 operator-(const vec3& vector)
	{
		return vec3(-vector.x, -vector.y, -vector.z);
	}

	vec3 vec3::Cross(const vec3& other) const
	{
		return vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	float vec3::Dot(const vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	float vec3::Magnitude() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	vec3 vec3::Normalize() const
	{
		float length = Magnitude();
		return vec3(x / length, y / length, z / length);
	}

	float vec3::Distance(const vec3& other) const
	{
		float a = x - other.x;
		float b = y - other.y;
		float c = z - other.z;
		return sqrt(a * a + b * b + c * c);
	}

	std::ostream& operator<<(std::ostream& stream, const vec3& vector)
	{
		stream << "vec3: (" << vector.x << ", " << vector.y << ", " << vector.z << ")";
		return stream;
	}


} }