#include "Scene.h"

#include "Renderer3D.h"

namespace sp { namespace graphics {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		for (uint i = 0; i < m_Meshes.size(); i++)
			delete m_Meshes[i];

		m_Meshes.clear();
	}

	void Scene::Add(Mesh* mesh)
	{
		m_Meshes.push_back(mesh);
	}

	void Scene::Render(Renderer3D& renderer)
	{
		for (Mesh* mesh : m_Meshes)
			renderer.Submit(mesh);
	}

} }