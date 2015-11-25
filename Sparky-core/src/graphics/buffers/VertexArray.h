#pragma once

#include <vector>
#include <sparky_types.h>

#include "Buffer.h"
#include "graphics/SPRenderAPI.h"

namespace sparky { namespace graphics {

	class VertexArray
	{
	private:
		static uint s_CurrentBinding;
	private:
		uint m_ID;
		std::vector<API::Buffer*> m_Buffers;
	public:
		VertexArray();
		~VertexArray();

		API::Buffer* GetBuffer(uint index = 0);

		void PushBuffer(API::Buffer* buffer);
		void Bind() const;
		void Unbind() const;
	};

} }