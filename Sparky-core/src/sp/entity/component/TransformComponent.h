#pragma once

#include "Component.h"

#include "sp/Common.h"
#include <sp/maths/maths.h>

namespace sp { namespace entity { namespace component {

	class SP_API TransformComponent : public Component
	{
	public:
		maths::mat4 transform;
	public:
		TransformComponent(Entity* entity, const maths::mat4& transform);

		static ComponentType* GetStaticType()
		{
			static ComponentType type({ "Transform" });
			return &type;
		}

		inline virtual ComponentType* GetType() const override { return GetStaticType(); }
	};

} } }