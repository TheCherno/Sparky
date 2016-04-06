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
	public:
		D3DVertexBuffer(BufferUsage usage);
		~D3DVertexBuffer();

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