#include "sp/sp.h"
#include "mat4.h"

namespace sp { namespace maths {

	String mat4::ToString() const
	{
		std::stringstream result;
		result << "mat4: (" << rows[0].x << ", " << rows[1].x << ", " << rows[2].x << ", " << rows[3].x << "), ";
		result << "(" << rows[0].y << ", " << rows[1].y << ", " << rows[2].y << ", " << rows[3].y << "), ";
		result << "(" << rows[0].z << ", " << rows[1].z << ", " << rows[2].z << ", " << rows[3].z << "), ";
		result << "(" << rows[0].w << ", " << rows[1].w << ", " << rows[2].w << ", " << rows[3].w << ")";
		return result.str();
	}

} }