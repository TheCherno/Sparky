#include "PostEffects.h"

namespace sparky { namespace graphics {

	PostEffects::PostEffects()
	{

	}

	PostEffects::~PostEffects()
	{

	}

	void PostEffects::Push(PostEffectsPass* pass)
	{
		m_Passes.push_back(pass);
	}

	void PostEffects::Pop()
	{
		m_Passes.pop_back();
	}

	void PostEffects::RenderPostEffects(Framebuffer* source, Framebuffer* target, uint quad, IndexBuffer* indices)
	{
		target->Bind();
		GLCall(glActiveTexture(GL_TEXTURE0));
		source->GetTexture()->Bind();

		GLCall(glBindVertexArray(quad));
		indices->Bind();

		for (PostEffectsPass* pass : m_Passes)
			pass->RenderPass(target);

		indices->Unbind();
		GLCall(glBindVertexArray(0));
	}

} }