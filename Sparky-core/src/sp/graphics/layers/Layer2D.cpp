#include "sp/sp.h"
#include "Layer2D.h"

#include "../BatchRenderer2D.h"
#include "sp/app/Window.h"

namespace sp { namespace graphics {

	Layer2D::Layer2D(Shader* shader, const maths::mat4& projectionMatrix)
		: m_Renderer(new BatchRenderer2D(Window::GetWindowClass(nullptr)->GetWidth(), Window::GetWindowClass(nullptr)->GetHeight())), m_Shader(shader), m_ProjectionMatrix(projectionMatrix)
	{
		m_Shader->Bind();
		m_Shader->SetUniformMat4("pr_matrix", m_ProjectionMatrix);

#ifdef SPARKY_PLATFORM_WEB
		m_Shader->SetUniform1i("texture_0", 0);
		m_Shader->SetUniform1i("texture_1", 1);
		m_Shader->SetUniform1i("texture_2", 2);
		m_Shader->SetUniform1i("texture_3", 3);
		m_Shader->SetUniform1i("texture_4", 4);
		m_Shader->SetUniform1i("texture_5", 5);
		m_Shader->SetUniform1i("texture_6", 6);
		m_Shader->SetUniform1i("texture_7", 7);
#else
		int texIDs[] =
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31
		};

		m_Shader->SetUniform1iv("textures", texIDs, 32);
#endif
		m_Shader->Unbind();
	}

	Layer2D::~Layer2D()
	{
		delete m_Shader;
		delete m_Renderer;

		for (uint i = 0; i < m_Renderables.size(); i++)
			delete m_Renderables[i];
	}

	void Layer2D::Init()
	{
		OnInit(*m_Renderer, *m_Shader);
	}

	void Layer2D::OnInit(Renderer2D& renderer, Shader& shader)
	{
	}

	Renderable2D* Layer2D::Add(Renderable2D* renderable)
	{
		m_Renderables.push_back(renderable);
		return renderable;
	}

	Renderable2D* Layer2D::Submit(Renderable2D* renderable)
	{
		m_SubmittedRenderables.push_back(renderable);
		return renderable;
	}

	void Layer2D::OnRender()
	{
		m_Shader->Bind();
		m_Renderer->Begin();

		for (const Renderable2D* renderable : m_Renderables)
			renderable->Submit(m_Renderer);

		for (const Renderable2D* renderable : m_SubmittedRenderables)
			renderable->Submit(m_Renderer);

		m_Renderer->End();
		m_Renderer->Present();

		OnRender(*m_Renderer);

		m_SubmittedRenderables.clear();
	}

	void Layer2D::OnRender(Renderer2D& renderer)
	{
	}

} }