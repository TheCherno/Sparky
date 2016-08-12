#include "sp/sp.h"
#include "Rectangle.h"

#include "AABB.h"

namespace sp { namespace maths {

	Rectangle::Rectangle()
		: position(vec2()), size(vec2())
	{
	}

	Rectangle::Rectangle(const AABB& aabb)
		: position(vec2(aabb.min)), size(vec2(aabb.GetSize()))
	{
	}

	Rectangle::Rectangle(const vec2& position, const vec2& size)
		: position(position), size(size)
	{
	}

	Rectangle::Rectangle(float x, float y, float width, float height)
		: position(vec2(x, y)), size(vec2(width, height))
	{
	}

	bool Rectangle::Intersects(const Rectangle& other) const
	{
		return (size > other.position && position < other.size) || (position > other.size && size < other.position);
	}

	bool Rectangle::Contains(const vec2& point) const
	{
		return point > GetMinimumBound() && point < GetMaximumBound();
	}

	bool Rectangle::Contains(const vec3& point) const
	{
		return Contains(vec2(point));
	}

	bool Rectangle::operator==(const Rectangle& other) const
	{
		return position == other.position && size == other.size;
	}

	bool Rectangle::operator!=(const Rectangle& other) const
	{
		return !(*this == other);
	}

	bool Rectangle::operator<(const Rectangle& other) const
	{
		return size < other.size;
	}

	bool Rectangle::operator>(const Rectangle& other) const
	{
		return size > other.size;
	}

} }