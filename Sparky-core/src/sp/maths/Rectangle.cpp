#include "sp/sp.h"
#include "Rectangle.h"

namespace sp { namespace maths {

	Rectangle::Rectangle()
		: position(vec2()), size(vec2())
	{
	}

	Rectangle::Rectangle(const vec2& position, const vec2& size)
		: position(vec3(position)), size(vec3(size))
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
		return point > position && point < position + size;
	}

	bool Rectangle::Contains(const vec3& point) const
	{
		return Contains(vec2(point));
	}

	vec2 Rectangle::Center() const
	{
		return position + size * 0.5f;
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