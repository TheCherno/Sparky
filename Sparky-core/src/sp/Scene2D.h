#pragma once

#include "sp/sp.h"
#include "graphics/camera/OrthographicCamera.h"
#include "graphics/BatchRenderer2D.h"
#include "entity/Entity.h"

namespace sp {
 
	class SP_API Scene2D
	{
	private:
		graphics::OrthographicCamera* m_Camera;
		graphics::BatchRenderer2D* m_Renderer;

		std::vector<entity::Entity*> m_Entities;
	public:
		Scene2D();
		Scene2D(const maths::mat4& projectionMatrix);
		~Scene2D();

		void Add(entity::Entity* entity);

		virtual void OnUpdate();
		virtual void OnRender(graphics::BatchRenderer2D& renderer);

		void OnRender();

		inline graphics::BatchRenderer2D* GetRenderer() { return m_Renderer; }
		inline graphics::OrthographicCamera* GetCamera() { return m_Camera; }
	};

}