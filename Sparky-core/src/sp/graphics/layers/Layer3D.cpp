#include "sp/sp.h"
#include "Layer3D.h"

#include "../DeferredRenderer.h"

namespace sp { namespace graphics {

	Layer3D::Layer3D(Scene* scene, Renderer3D* renderer)
		: m_Scene(scene), m_Renderer(renderer)
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

	bool Layer3D::OnResize(uint width, uint height)
	{
		m_Renderer->SetScreenBufferSize(width, height);
		return false;
	}

	void Layer3D::OnUpdateInternal(const Timestep& ts)
	{
		OnUpdate(ts);
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