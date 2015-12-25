#include "sp/sp.h"
#include "Layer3D.h"

#include "../ForwardRenderer.h"

namespace sp { namespace graphics {

	Layer3D::Layer3D(Scene* scene)
		: m_Scene(scene), m_Renderer(new ForwardRenderer())
	{
	}

	Layer3D::~Layer3D()
	{
		delete m_Scene;
		delete m_Renderer;
	}

	void Layer3D::Init()
	{
		OnInit(*m_Renderer, *m_Scene);
	}

	void Layer3D::OnInit(Renderer3D& renderer, Scene& scene)
	{
	}

	void Layer3D::OnRender()
	{
		OnRender(*m_Renderer);
	}

	void Layer3D::OnRender(Renderer3D& renderer)
	{
		m_Scene->Render(renderer);
		renderer.Present();
	}

} }