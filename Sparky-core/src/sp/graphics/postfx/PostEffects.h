#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

#include "sp/graphics/API/Framebuffer.h"
#include "sp/graphics/API/VertexArray.h"
#include "sp/graphics/API/IndexBuffer.h"
#include "sp/graphics/Mesh.h"
#include "PostEffectsPass.h"

namespace sp { namespace graphics {

	// The Post Effects pipeline
	class SP_API PostEffects
	{
	private:
		std::vector<PostEffectsPass*> m_Passes;
	public:
		PostEffects();
		~PostEffects();
		void Push(PostEffectsPass* pass);
		PostEffectsPass* Pop();
		
		inline const std::vector<PostEffectsPass*>& GetPasses() const{ return m_Passes; }

		void RenderPostEffects(Framebuffer* source, Framebuffer* target, Mesh* quad);
	};

} }