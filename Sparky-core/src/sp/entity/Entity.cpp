#include "sp/sp.h"
#include "Entity.h"

namespace sp { namespace entity {

	using namespace component;

	Entity::Entity()
	{
	}

	Entity::Entity(graphics::Sprite* sprite, const maths::mat4& transform)
	{
		AddComponent(spnew SpriteComponent(sprite));
		AddComponent(spnew TransformComponent(transform));
	}

	Entity::Entity(graphics::Mesh* mesh, const maths::mat4& transform)
	{
		AddComponent(spnew MeshComponent(mesh));
		AddComponent(spnew TransformComponent(transform));
	}

	void Entity::AddComponent(component::Component* component)
	{
		SP_ASSERT(component->GetType());
		m_Components[component->GetType()] = component;
	}

} }