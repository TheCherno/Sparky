#pragma once

#include "sp/Common.h"
#include "../Framebuffer.h"
#include "../shaders/Shader.h"

namespace sp { namespace graphics {

	class SP_API PostEffectsPass
	{
	private:
		Shader* m_Shader;
	public:
		PostEffectsPass(Shader* shader);
		~PostEffectsPass();

		void RenderPass(Framebuffer* target);
	};

} }