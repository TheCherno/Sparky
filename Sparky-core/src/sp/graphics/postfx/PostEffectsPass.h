#pragma once

#include "sp/Common.h"
#include "sp/graphics/API/Framebuffer.h"
#include "../Material.h"

namespace sp { namespace graphics {

	class SP_API PostEffectsPass
	{
	private:
		Material* m_Material;
	public:
		expicit PostEffectsPass(API::Shader* shader);
		~PostEffectsPass();

		void RenderPass(Framebuffer* target);
	};

} }
