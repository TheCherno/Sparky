#include "simple2drenderer.h"

namespace sparky { namespace graphics {

	void Simple2DRenderer::submit(const Renderable2D* renderable)
	{
		m_RenderQueue.push_back(renderable);
	}

	void Simple2DRenderer::flush()
	{
		while (!m_RenderQueue.empty())
		{
			const Renderable2D* renderable = m_RenderQueue.front();
			renderable->getVAO()->bind();
			renderable->getIBO()->bind();

			renderable->getShader().setUniformMat4("ml_matrix", maths::mat4::translation(renderable->getPosition()));
			glDrawElements(GL_TRIANGLES, renderable->getIBO()->getCount(), GL_UNSIGNED_SHORT, nullptr);

			renderable->getIBO()->unbind();
			renderable->getVAO()->unbind();

			m_RenderQueue.pop_front();
		}
	}


} }