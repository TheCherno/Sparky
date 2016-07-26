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
		void* m_Data;
	public:
		GLVertexBuffer(BufferUsage usage);
		~GLVertexBuffer();

		void Resize(uint size) override;
		void SetLayout(const BufferLayout& layout) override;
		void SetData(uint size, const void* data) override;

		void ReleasePointer() override;

		void Bind() override;
		void Unbind() override;

		inline const void* GetData() const { return m_Data; }
		inline uint GetSize() const { return m_Size; }
		inline const BufferLayout& GetLayout() const { return m_Layout; }
	protected:
		void* GetPointerInternal() override;
	};


} } }