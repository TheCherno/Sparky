#include "layer.h"

namespace sparky { namespace graphics {

	Layer::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix)
		: m_Renderer(renderer), m_Shader(shader), m_ProjectionMatrix(projectionMatrix)
	{
		m_Shader->enable();
		m_Shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);

#ifdef SPARKY_EMSCRIPTEN
		m_Shader->setUniform1i("texture_0", 0);
		m_Shader->setUniform1i("texture_1", 1);
		m_Shader->setUniform1i("texture_2", 2);
		m_Shader->setUniform1i("texture_3", 3);
		m_Shader->setUniform1i("texture_4", 4);
		m_Shader->setUniform1i("texture_5", 5);
		m_Shader->setUniform1i("texture_6", 6);
		m_Shader->setUniform1i("texture_7", 7);
#else
		GLint texIDs[] =
		{
			 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31
		};

		m_Shader->setUniform1iv("textures", texIDs, 32);
#endif
		m_Shader->disable();
	}

	Layer::~Layer()
	{
		delete m_Shader;
		delete m_Renderer;

		for (int i = 0; i < m_Renderables.size(); i++)
			delete m_Renderables[i];
	}

	void Layer::add(Renderable2D* renderable)
	{
		m_Renderables.push_back(renderable);
	}

	void Layer::render()
	{
		m_Shader->enable();
		m_Renderer->begin();

		for (const Renderable2D* renderable : m_Renderables)
			renderable->submit(m_Renderer);

		m_Renderer->end();
		m_Renderer->flush();
	}

} }