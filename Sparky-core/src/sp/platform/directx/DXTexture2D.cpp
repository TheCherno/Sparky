#include "sp/sp.h"
#include "DXTexture2D.h"

#include "DXCommon.h"
#include "DXContext.h"
#include "DXTextureHandle.h"

#include "sp/utils/ImageLoad.h"
#include "sp/utils/Log.h"
#include "sp/system/Memory.h"

#include "sp/utils/ImageLoad.h"

namespace sp { namespace graphics { namespace API {

	D3DTexture2D::D3DTexture2D(uint width, uint height, TextureParameters parameters, TextureLoadOptions loadOptions)
		: m_FileName("NULL")
	{
		m_Width = width;
		m_Height = height;
		m_Parameters = parameters;
		m_LoadOptions = loadOptions;
		Load();
	}

	D3DTexture2D::D3DTexture2D(uint width, uint height, uint color, TextureParameters parameters, TextureLoadOptions loadOptions)
		: m_FileName("NULL")
	{
		m_Width = width;
		m_Height = height;
		m_Parameters = parameters;
		m_LoadOptions = loadOptions;
		Load();

		SetData(color);
	}

	D3DTexture2D::D3DTexture2D(const String& name, const String& filename, TextureParameters parameters, TextureLoadOptions loadOptions)
		: m_Name(name), m_FileName(filename)
	{
		m_Parameters = parameters;
		m_LoadOptions = loadOptions;
		Load();
	}

	D3DTexture2D::~D3DTexture2D()
	{
		// GLCall(glDeleteTextures(1, &m_TID));
	}

	D3DTexture2D* D3DTexture2D::FromFile(const String& filepath)
	{
		// TODO: Set name to filename
		return spnew D3DTexture2D(filepath, filepath);
	}

	void D3DTexture2D::Load()
	{
		byte* data = nullptr;
		if (m_FileName != "NULL")
		{
			data = LoadImage(m_FileName, &m_Width, &m_Height, &m_BitsPerPixel, !m_LoadOptions.flipY); // FreeImage loads bottom->top
			m_Parameters.format = m_BitsPerPixel == 24 ? TextureFormat::RGB : TextureFormat::RGBA;
		}

		uint stride = 4;// GetStrideFromFormat(m_Parameters.format);
		D3D11_SUBRESOURCE_DATA* initData = nullptr;
		if (data)
		{
			initData = spnew D3D11_SUBRESOURCE_DATA[1]; // TODO: Mips
			initData[0].pSysMem = data;
			initData[0].SysMemPitch = stride * m_Width;
			initData[0].SysMemSlicePitch = m_Width * m_Height * stride;
		}

		bool generateMips = data != nullptr;

		ZeroMemory(&m_Desc, sizeof(D3D11_TEXTURE2D_DESC));
		m_Desc.Width = m_Width;
		m_Desc.Height = m_Height;
		m_Desc.MipLevels = 1;
		m_Desc.ArraySize = 1;
		m_Desc.Format = SPTextureFormatToD3D(m_Parameters.format);
		m_Desc.CPUAccessFlags = 0;
		m_Desc.Usage = data ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = m_Desc.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (generateMips)
			m_Desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		m_Desc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
		m_Desc.SampleDesc.Count = 1;
		m_Desc.SampleDesc.Quality = 0;

		DXCall(D3DContext::GetDevice()->CreateTexture2D(&m_Desc, initData, &m_Texture));
		// SP_ASSERT(hr);
		// SP_ASSERT(result->texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = m_Desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = generateMips ? -1 : m_Desc.MipLevels;

		DXCall(D3DContext::GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView));
		if (generateMips)
			D3DContext::GetDeviceContext()->GenerateMips(m_ResourceView);

		m_Desc.Usage = D3D11_USAGE_DEFAULT;
		m_Desc.CPUAccessFlags = 0;
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.MinLOD = 0;
		m_SamplerDesc.MaxLOD = 11;
		m_SamplerDesc.Filter = m_Parameters.filter == TextureFilter::LINEAR ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		DXCall(D3DContext::GetDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState));

		if (data != nullptr)
			spdel[] data;
	}

	void D3DTexture2D::Bind(uint slot) const
	{
		D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		D3DContext::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void D3DTexture2D::Unbind(uint slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void D3DTexture2D::SetData(const void* pixels)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DXCall(D3DContext::GetDeviceContext()->Map(m_Texture, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr));
		for (uint i = 0; i < m_Width * m_Height * GetStrideFromFormat(m_Parameters.format); i += 4)
		{
			((byte*)msr.pData)[i + 0] = 0xff;
			((byte*)msr.pData)[i + 1] = 0xff;
			((byte*)msr.pData)[i + 2] = 0xff;
			((byte*)msr.pData)[i + 3] = ((byte*)pixels)[i / 2 + 1];
		}
		D3DContext::GetDeviceContext()->Unmap(m_Texture, NULL);
	}

	DXGI_FORMAT D3DTexture2D::SPTextureFormatToD3D(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:
		case TextureFormat::RGBA:
		case TextureFormat::LUMINANCE_ALPHA:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	uint D3DTexture2D::SPTextureFilterToD3D(TextureFilter filter)
	{
		return 0;
	}

	uint D3DTexture2D::SPTextureWrapToD3D(TextureWrap wrap)
	{
		return 0;
	}

} } }