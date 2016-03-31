#include "sp/sp.h"
#include "DXVertexArray.h"

#include "DXCommon.h"
#include "DXContext.h"
#include "DXVertexBuffer.h"

namespace sp { namespace graphics { namespace API {

	D3DVertexArray::D3DVertexArray()
	{
	}

	D3DVertexArray::~D3DVertexArray()
	{
	}

	void D3DVertexArray::PushBuffer(API::VertexBuffer* buffer)
	{
		m_Buffers.push_back(buffer);
	}

	void D3DVertexArray::Bind() const
	{
	}

	void D3DVertexArray::Unbind() const
	{
	}

	void D3DVertexArray::Draw(uint count) const
	{
		// TODO: For now
		for (uint i = 0; i < m_Buffers.size(); i++)
		{
			D3DVertexBuffer* buffer = (D3DVertexBuffer*)m_Buffers[i];
			buffer->Bind();

			D3DContext::GetDeviceContext()->DrawIndexed(count, 0, 0);

			buffer->Unbind();
		}
	}

} } }
