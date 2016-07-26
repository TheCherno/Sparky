#include "sp/sp.h"
#include "TransformComponent.h"

namespace sp { namespace entity { namespace component {

	TransformComponent::TransformComponent(const maths::mat4& transform)
		: transform(transform)
	{
	}

} } }