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
		PostEffectsPass(Material* shader);
		~PostEffectsPass();

		inline Material* GetMaterial() { return m_Material; }

		void RenderPass(Framebuffer* target);
	};

} }