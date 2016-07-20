#pragma once

#include "Component.h"

#include "sp/Common.h"
#include "sp/graphics/Mesh.h"

namespace sp { namespace entity { namespace component {

	class SP_API MeshComponent : public Component
	{
	public:
		graphics::Mesh* mesh;
	public:
		MeshComponent(Entity* entity, graphics::Mesh* mesh = nullptr);

		static ComponentType* GetStaticType()
		{
			static ComponentType type({ "Mesh" });
			return &type;
		}

		inline virtual ComponentType* GetType() const override { return GetStaticType(); }
	};

} } }
