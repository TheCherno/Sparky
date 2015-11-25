#pragma once

#include <iostream>

#include "vec2.h"
#include "vec3.h"

namespace sp { namespace maths {

	struct AABB
	{
		vec3 min, max;

		AABB();
		AABB(const vec2& min, const vec2& max);
		AABB(const vec3& min, const vec3& max);

		bool Intersects(const AABB& other) const;
		bool Contains(const vec2& point) const;
		bool Contains(const vec3& point) const;

		vec3 Center() const;

		bool operator==(const AABB& other) const;
		bool operator!=(const AABB& other) const;

		bool operator<(const AABB& other) const;
		bool operator>(const AABB& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const AABB& aabb);
	};

} }