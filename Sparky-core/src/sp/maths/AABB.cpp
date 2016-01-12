#include "sp/sp.h"
#include "AABB.h"

namespace sp { namespace maths {

	AABB::AABB()
		: min(vec3()), max(vec3())
	{
	}

	AABB::AABB(const vec2& min, const vec2& max)
		: min(vec3(min)), max(vec3(max))
	{
	}

	AABB::AABB(const vec3& min, const vec3& max)
		: min(min), max(max)
	{
	}

	AABB::AABB(float x, float y, float width, float height)
		: min(vec3(x, y, 0.0f)), max(vec3(width, height, 0.0f))
	{
	}

	AABB::AABB(float x, float y, float z, float width, float height, float depth)
		: min(vec3(x, y, z)), max(vec3(width, height, depth))
	{
	}

	bool AABB::Intersects(const AABB& other) const
	{
		return (max > other.min && min < other.max) || (min > other.max && max < other.min);
	}

	bool AABB::Contains(const vec2& point) const
	{
		return vec3(point) > min && vec3(point) < max;
	}

	bool AABB::Contains(const vec3& point) const
	{
		return point > min && point < max;
	}

	vec3 AABB::Center() const
	{
		return (min - max) * 0.5f;
	}

	bool AABB::operator==(const AABB& other) const
	{
		return min == other.min && max == other.max;
	}

	bool AABB::operator!=(const AABB& other) const
	{
		return !(*this == other);
	}

	bool AABB::operator<(const AABB& other) const
	{
		return max < other.min;
	}

	bool AABB::operator>(const AABB& other) const
	{
		return min > other.max;
	}

} }