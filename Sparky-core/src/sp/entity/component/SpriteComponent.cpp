#include "sp/sp.h"
#include "SpriteComponent.h"

namespace sp { namespace entity { namespace component {

	SpriteComponent::SpriteComponent(Entity* entity, graphics::Sprite* sprite)
		: Component(entity), sprite(sprite)
	{
	}

} } }