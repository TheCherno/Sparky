#pragma once

#include <vector>

#include <sp/Types.h>
#include <sp/entity/Entity.h>

#include "camera/Camera.h"

namespace sp { namespace graphics {

	class Renderer3D;

	// TODO: This probably shouldn't be in graphics.
	class SP_API Scene
	{
	private:
		std::vector<entity::Entity*> m_Entities;
		Camera* m_Camera;
	public:
		Scene();
		Scene(Camera* camera);
		~Scene();
		void Add(entity::Entity* entity);
		void Update();
		void Render(Renderer3D& renderer);

		inline Camera* GetCamera() const { return m_Camera; }
		inline const std::vector<entity::Entity*>& GetEntities() const { return m_Entities; }
	};

} }
