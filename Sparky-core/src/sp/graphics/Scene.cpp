#include "Scene.h"

#include "Renderer3D.h"

#include "camera/MayaCamera.h"

namespace sp { namespace graphics {

	using namespace entity;
	using namespace component;

	Scene::Scene()
		: m_Camera(new MayaCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)))
	{
	}

	Scene::Scene(Camera* camera)
		: m_Camera(camera)
	{
	}

	Scene::~Scene()
	{
		for (uint i = 0; i < m_Entities.size(); i++)
			delete m_Entities[i];

		m_Entities.clear();
	}

	void Scene::Add(Entity* entity)
	{
		m_Entities.push_back(entity);
		if (!entity->GetComponent<TransformComponent>())
		{
			SPARKY_WARN("Entity does not have Transform, creating...");
			entity->AddComponent(new TransformComponent(maths::mat4::Identity()));
		}
	}

	void Scene::Update()
	{
	}

	void Scene::Render(Renderer3D& renderer)
	{
		Camera* camera = m_Camera;
		camera->Update();

		renderer.Begin();

		for (Entity* entity : m_Entities)
		{
			MeshComponent* mesh = entity->GetComponent<MeshComponent>();
			if (mesh)
			{
				TransformComponent* tc = entity->GetComponent<TransformComponent>();
				SPARKY_ASSERT(tc, "Mesh does not have transform!"); // Meshes MUST have transforms
				renderer.SubmitMesh(camera, mesh->mesh, tc->transform);
			}
		}

		renderer.End();
	}

} }