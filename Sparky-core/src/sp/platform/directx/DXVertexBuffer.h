#pragma once

#include "sp/graphics/API/VertexBuffer.h"
#include "DXCommon.h"

namespace sp { namespace graphics { namespace API {

	class D3DVertexBuffer : public VertexBuffer
	{
	private:
		D3D11_BUFFER_DESC m_BufferDesc;
		D3D11_MAPPED_SUBRESOURCE m_MappedSubresource;
		ID3D11Buffer* m_BufferHandle;
		ID3D11InputLayout* m_InputLayout;

		BufferUsage m_Usage;
		uint m_Size;
		BufferLayout m_Layout;
		void* m_Data; // TODO: Flags should determine whether vertex buffer data persists CPU-side
	public:
		D3DVertexBuffer(BufferUsage usage);
		~D3DVertexBuffer();

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