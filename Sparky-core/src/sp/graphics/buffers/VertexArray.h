#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "Buffer.h"

namespace sp { namespace graphics {

	class SP_API VertexArray
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

		// TODO: I don't like this
		void Draw(uint count) const;
	};

} }