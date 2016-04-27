#include "sp/sp.h"
#include "PostEffectsPass.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	struct PostEffectsPassShader
	{
		maths::mat4 pr_matrix;
		API::Texture* texture;
	};

	PostEffectsPass::PostEffectsPass(Material* material)
		: m_Material(material)
	{
	}

	PostEffectsPass::~PostEffectsPass()
	{

	}

	void PostEffectsPass::RenderPass(Framebuffer* target)
	{
	}

} }