#include "sp/sp.h"
#include "DXIndexBuffer.h"

#include "DXContext.h"

namespace sp { namespace graphics { namespace API {

	D3DIndexBuffer::D3DIndexBuffer(uint16* data, uint count)
		: m_Count(count)
	{
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = count * sizeof(uint16);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibInitData;
		ibInitData.pSysMem = data;
		HRESULT hr = D3DContext::GetDevice()->CreateBuffer(&ibd, &ibInitData, &m_Handle);
	}

	D3DIndexBuffer::D3DIndexBuffer(uint* data, uint count)
		: m_Count(count)
	{
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = count * sizeof(uint);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibInitData;
		ibInitData.pSysMem = data;
		HRESULT hr = D3DContext::GetDevice()->CreateBuffer(&ibd, &ibInitData, &m_Handle);
	}

	D3DIndexBuffer::~D3DIndexBuffer()
	{
		// TODO
	}

	void D3DIndexBuffer::Bind() const
	{
		D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3DContext::GetDeviceContext()->IASetIndexBuffer(m_Handle, DXGI_FORMAT_R32_UINT, 0);
	}

	void D3DIndexBuffer::Unbind() const
	{
		// TODO
	}

	uint D3DIndexBuffer::GetCount() const
	{
		return m_Count;
	}

} } }