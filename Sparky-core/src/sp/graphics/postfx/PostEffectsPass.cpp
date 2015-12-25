#include "sp/sp.h"
#include "PostEffectsPass.h"

#include <sp/graphics/SPRenderAPI.h>

namespace sp { namespace graphics {

	PostEffectsPass::PostEffectsPass(Shader* shader)
		: m_Shader(shader)
	{
		m_Shader->Bind();
		m_Shader->SetUniform1i("tex", 0);
		m_Shader->Unbind();
	}

	PostEffectsPass::~PostEffectsPass()
	{

	}

	void PostEffectsPass::RenderPass(Framebuffer* target)
	{
		m_Shader->Bind();
		m_Shader->SetUniformMat4("pr_matrix", maths::mat4::Orthographic(0, target->GetWidth(), target->GetHeight(), 0, -1.0f, 1.0f));
		API::DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		m_Shader->Unbind();
	}

} }