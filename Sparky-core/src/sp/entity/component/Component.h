#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"

namespace sp { namespace entity {
	class Entity;
} }

namespace sp { namespace entity { namespace component {

	struct SP_API ComponentType
	{
		String name;
	};

	class SP_API Component
	{
	protected:
		Entity* m_Entity;
	public:
		virtual Entity* GetEntity() { return m_Entity; }
		virtual ComponentType* GetType() const { return nullptr; }
	};

} } }
