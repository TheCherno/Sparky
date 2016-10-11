#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

#include "component/Components.h"

namespace sp { namespace entity {

	class SP_API Entity
	{
	protected:
		std::unordered_map<component::ComponentType*, component::Component*> m_Components;
	public:
		Entity();
		Entity(graphics::Sprite* sprite, const maths::mat4& transform = maths::mat4::Identity());
		Entity(graphics::Mesh* mesh, const maths::mat4& transform = maths::mat4::Identity());

		void AddComponent(component::Component* component);

		template <typename T>
		const T* GetComponent() const
		{
			return GetComponentInternal<T>();
		}

		template <typename T>
		T* GetComponent()
		{
			return (T*)GetComponentInternal<T>();
		}
	private:
		template <typename T>
		const T* GetComponentInternal() const
		{
			component::ComponentType* type = T::GetStaticType();
			auto it = m_Components.find(type);
			if (it == m_Components.end())
				return nullptr;
			return (T*)it->second; // TODO: ...
		}
	};

} }
