#include "sp/sp.h"
#include "TransformComponent.h"

namespace sp { namespace entity { namespace component {

	TransformComponent::TransformComponent(Entity* entity, const maths::mat4& transform)
		: Component(entity), transform(transform)
	{
	}

} } }