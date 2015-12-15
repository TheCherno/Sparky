#include "Entity.h"

namespace sp { namespace entity {

	Entity::Entity()
	{
	}

	void Entity::AddComponent(component::Component* component)
	{
		m_Components.push_back(component);
	}

} }