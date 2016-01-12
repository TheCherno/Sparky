#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

#include "vec2.h"
#include "vec3.h"

namespace sp { namespace maths {

	struct SP_API AABB
	{
		vec3 min;
		vec3 max;

		AABB();
		AABB(const vec2& min, const vec2& max);
		AABB(const vec3& min, const vec3& max);
		AABB(float x, float y, float width, float height);
		AABB(float x, float y, float z, float width, float height, float depth);

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