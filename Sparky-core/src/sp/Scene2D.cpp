#include "sp/sp.h"
#include "Scene2D.h"

#include "app/Application.h"
#include "sp/debug/DebugRenderer.h"

namespace sp {

	using namespace graphics;
	using namespace maths;

	using namespace entity;
	using namespace component;
 
	Scene2D::Scene2D()
	{
		float width = Application::GetApplication().GetWindowWidth();
		float height = Application::GetApplication().GetWindowHeight();
		float aspect = width / height;

		m_Camera = spnew OrthographicCamera(mat4::Orthographic(-10.0f * aspect, 10.0f * aspect, -10.0f, 10.0f, -1.0f, 1.0f));
		m_Renderer = spnew BatchRenderer2D(width, height);
		m_Renderer->SetCamera(m_Camera);
	}

	Scene2D::Scene2D(const mat4& projectionMatrix)
		: m_Camera(spnew OrthographicCamera(projectionMatrix))
	{
		float width = Application::GetApplication().GetWindowWidth();
		float height = Application::GetApplication().GetWindowHeight();

		m_Renderer = spnew BatchRenderer2D(width, height);
		m_Renderer->SetCamera(m_Camera);
	}

	Scene2D::~Scene2D()
	{
		spdel m_Camera;
	}

	void Scene2D::Add(Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	void Scene2D::OnUpdate()
	{
		// for (uint i = 0; i < m_Entities.size(); i++)
		// 	m_Entities[i]
	}

	void Scene2D::OnRender(BatchRenderer2D& renderer)
	{
	
	}

	void Scene2D::OnRender()
	{
		Camera* camera = m_Camera;
		camera->Update();
		debug::DebugRenderer::SetCamera(camera);

		m_Renderer->Begin();
		for (uint i = 0; i < m_Entities.size(); i++)
		{
			Entity* entity = m_Entities[i];
			SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
			if (sprite)
			{
				TransformComponent* tc = entity->GetComponent<TransformComponent>();
				SP_ASSERT(tc, "Sprite does not have transform!"); // Sprites MUST have transforms
				sprite->sprite->Submit(m_Renderer);
			}

		}
		OnRender(*m_Renderer);

		m_Renderer->End();
		m_Renderer->Present();
	}

}