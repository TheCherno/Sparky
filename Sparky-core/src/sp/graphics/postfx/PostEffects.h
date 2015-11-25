#pragma once

#include <vector>

#include "../Framebuffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/IndexBuffer.h"
#include "PostEffectsPass.h"

namespace sp { namespace graphics {

	// The Post Effects pipeline
	class PostEffects
	{
	private:
		std::vector<PostEffectsPass*> m_Passes;
	public:
		PostEffects();
		~PostEffects();
		void Push(PostEffectsPass* pass);
		void Pop();

		void RenderPostEffects(Framebuffer* source, Framebuffer* target, VertexArray* quad, IndexBuffer* indices);
	};

} }