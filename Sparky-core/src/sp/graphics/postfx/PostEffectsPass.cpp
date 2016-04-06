#include "sp/sp.h"
#include "PostEffectsPass.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	struct PostEffectsPassShader
	{
		maths::mat4 pr_matrix;
		API::Texture* texture;
	};

	PostEffectsPass::PostEffectsPass(API::Shader* shader)
	{
		m_Material = spnew Material(shader);
		// m_Material->SetTexture("tex", 0);
	}

	PostEffectsPass::~PostEffectsPass()
	{

	}

	void PostEffectsPass::RenderPass(Framebuffer* target)
	{
		m_Material->SetUniform("pr_matrix", maths::mat4::Orthographic(0.0f, (float)target->GetWidth(), (float)target->GetHeight(), 0.0f, -1.0f, 1.0f));
		m_Material->Bind();
		// API::DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}

} }