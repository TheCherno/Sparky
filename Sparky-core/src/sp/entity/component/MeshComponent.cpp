#include "sp/sp.h"
#include "MeshComponent.h"

namespace sp { namespace entity { namespace component {

	MeshComponent::MeshComponent(Entity* entity, graphics::Mesh* mesh)
		: Component(entity), mesh(mesh)
	{
	}

} } }