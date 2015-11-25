#pragma once

#include <sp/Types.h>
#include "../SPRenderAPI.h"

namespace sp { namespace graphics {

	class IndexBuffer
	{
	private:
		uint m_Count;
		API::Buffer* m_Buffer;
	public:
		IndexBuffer(short* data, uint count);
		IndexBuffer(uint* data, uint count);
		~IndexBuffer();
		void Bind() const;
		void Unbind() const;

		inline uint GetCount() const { return m_Count; }
	};

} }