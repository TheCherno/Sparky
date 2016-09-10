#pragma once

#include "sp/graphics/API/VertexBuffer.h"
#include "GLCommon.h"

namespace sp { namespace graphics { namespace API {

	class GLVertexBuffer : public VertexBuffer
	{
	private:
		uint m_Handle;
		BufferUsage m_Usage;
		uint m_Size;
		BufferLayout m_Layout;
		byte* m_ScratchBuffer;
	public:
		GLVertexBuffer(BufferUsage usage);
		~GLVertexBuffer();

		void Resize(uint size) override;
		void SetLayout(const BufferLayout& layout) override;
		void SetData(uint size, const void* data) override;

		void ReleasePointer() override;

		void Bind() override;
		void Unbind() override;
	protected:
		void* GetPointerInternal() override;
	};


} } }