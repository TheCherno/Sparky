#include "sp/sp.h"
#include "DXVertexBuffer.h"

#include "DXContext.h"
#include "DXShader.h"

namespace sp { namespace graphics { namespace API {

	uint SPBufferUsageToDirect3D(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::STATIC:	return /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DYNAMIC;
		case BufferUsage::DYNAMIC:	return D3D11_USAGE_DYNAMIC;
		}
		return 0;
	}

	D3DVertexBuffer::D3DVertexBuffer(BufferUsage usage)
		: m_Usage(usage), m_Size(0)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = (D3D11_USAGE)SPBufferUsageToDirect3D(usage);     // write access access by CPU and GPU
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	}

	D3DVertexBuffer::~D3DVertexBuffer()
	{
		// GLCall(glDeleteBuffers(1, &id));
	}

	void D3DVertexBuffer::Resize(uint size)
	{
		m_Size = size;
		m_BufferDesc.ByteWidth = size;
		DXCall(D3DContext::GetContext()->GetDevice()->CreateBuffer(&m_BufferDesc, NULL, &m_BufferHandle));
	}

	void D3DVertexBuffer::SetLayout(const BufferLayout& bufferLayout)
	{
		m_Layout = bufferLayout;
		const std::vector<BufferElement>& layout = bufferLayout.GetLayout();
		D3D11_INPUT_ELEMENT_DESC* desc = spnew D3D11_INPUT_ELEMENT_DESC[layout.size()];
		for (uint i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			desc[i] = { element.name.c_str(), 0, (DXGI_FORMAT)element.type, 0, element.offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		const D3DShader* shader = D3DShader::CurrentlyBound();
		SP_ASSERT(shader);
		DXCall(D3DContext::GetDevice()->CreateInputLayout(desc, layout.size(), shader->GetData().vs->GetBufferPointer(), shader->GetData().vs->GetBufferSize(), &m_InputLayout));
		spdel desc;
	}

	void D3DVertexBuffer::SetData(uint size, const void* data)
	{
		if (m_Size < size)
			Resize(size);

		GetPointerInternal();
		memcpy(m_MappedSubresource.pData, data, size);
		ReleasePointer();
	}

	void* D3DVertexBuffer::GetPointerInternal()
	{
		DXCall(D3DContext::GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedSubresource));
		return m_MappedSubresource.pData;
	}

	void D3DVertexBuffer::ReleasePointer()
	{
		D3DContext::GetDeviceContext()->Unmap(m_BufferHandle, NULL);
	}

	void D3DVertexBuffer::Bind()
	{
		uint offset = 0;
		uint stride = m_Layout.GetStride();
		D3DContext::GetDeviceContext()->IASetInputLayout(m_InputLayout);
		D3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_BufferHandle, &stride, &offset);
	}

	void D3DVertexBuffer::Unbind()
	{
	}

} } }

