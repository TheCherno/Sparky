#include "sp/sp.h"
#include "PostEffects.h"

namespace sp { namespace graphics {

	PostEffects::PostEffects()
	{

	}

	PostEffects::~PostEffects()
	{
		for (PostEffectsPass* pass : m_Passes)
			delete pass;
	}

	void PostEffects::Push(PostEffectsPass* pass)
	{
		m_Passes.push_back(pass);
	}

	PostEffectsPass* PostEffects::Pop()
	{
		PostEffectsPass* pass = m_Passes.back();
		m_Passes.pop_back();
		return pass;
	}

	void PostEffects::RenderPostEffects(Framebuffer* source, Framebuffer* target, Mesh* quad)
	{

	}

} }