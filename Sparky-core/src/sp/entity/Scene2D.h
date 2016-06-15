#pragma once

namespace sp {
 
	class Scene2D
	{
	private:
		std::vector<entity::Entity*> m_Entities;
		Camera* m_Camera;
	public:
		Scene2D();
		Scene2D(Camera* camera);
		~Scene2D();

		void Add(entity::Entity* entity);
		void SetCamera(Camera* camera);

		void Update();
		void Render(Renderer2D& renderer);

		inline Camera* GetCamera() const { return m_Camera; }
		inline const std::vector<entity::Entity*>& GetEntities() const { return m_Entities; }
	};

}