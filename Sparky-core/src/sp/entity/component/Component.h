#pragma once

#include <sp/Types.h>

namespace sp { namespace entity {
	class Entity;
} }

namespace sp { namespace entity { namespace component {

	struct ComponentType
	{
		String name;
	};

	class Component
	{
	protected:
		Entity* m_Entity;
	public:
		virtual Entity* GetEntity() { return m_Entity; }
		virtual ComponentType* GetType() const { return nullptr; }
	};

} } }
