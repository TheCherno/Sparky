#include "sp/sp.h"
#include "MeshComponent.h"

namespace sp { namespace entity { namespace component {

	MeshComponent::MeshComponent(graphics::Mesh* mesh)
		: mesh(mesh)
	{
	}

} } }