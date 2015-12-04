#include "ForwardRenderer.h"

namespace sp { namespace graphics {

	ForwardRenderer::ForwardRenderer()
	{

	}

	void ForwardRenderer::Init()
	{

	}

	void ForwardRenderer::Begin()
	{
		m_DrawQueue.clear();
	}

	void ForwardRenderer::Submit(Mesh* mesh)
	{
		m_DrawQueue.push_back(mesh);
	}

	void ForwardRenderer::End()
	{

	}

	void ForwardRenderer::Present()
	{
		for (uint i = 0; i < m_DrawQueue.size(); i++)
			m_DrawQueue[i]->Render(*this);
	}

} }