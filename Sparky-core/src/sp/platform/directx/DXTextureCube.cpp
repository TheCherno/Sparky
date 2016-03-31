#include "sp/sp.h"
#include "DXTextureCube.h"

#include "DXContext.h"

#include "sp/utils/ImageLoad.h"
#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	D3DTextureCube::D3DTextureCube(const String& name, const String& filepath)
	{
		m_Name = name;
		m_Files[0] = filepath;
		LoadFromSingleFile();
	}

	D3DTextureCube::D3DTextureCube(const String& name, const String* files)
	{
		m_Name = name;
		m_Files = spnew String[6];
		for (uint i = 0; i < 6; i++)
			m_Files[i] = files[i];
		LoadFromMultipleFiles();
	}

	D3DTextureCube::D3DTextureCube(const String& name, const String* files, int32 mips, InputFormat format)
	{
		m_Name = name;
		m_Files = spnew String[mips];
		for (int32 i = 0; i < mips; i++)
			m_Files[i] = files[i];
		if (format == InputFormat::VERTICAL_CROSS)
			LoadFromVCross(mips);
	}

	D3DTextureCube::~D3DTextureCube()
	{
		// GLCall(glDeleteTextures(1, &m_Handle));
	}

	void D3DTextureCube::Bind(uint slot) const
	{
		D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		D3DContext::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void D3DTextureCube::Unbind(uint slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void D3DTextureCube::LoadFromSingleFile()
	{
		// TODO: Implement
	}

	void D3DTextureCube::LoadFromMultipleFiles()
	{
		const String& xpos = m_Files[0];
		const String& xneg = m_Files[1];
		const String& ypos = m_Files[2];
		const String& yneg = m_Files[3];
		const String& zpos = m_Files[4];
		const String& zneg = m_Files[5];

		uint width, height, bits;
		byte* xp = LoadImage(xpos, &width, &height, &bits, true);
		byte* xn = LoadImage(xneg, &width, &height, &bits, true);
		byte* yp = LoadImage(ypos, &width, &height, &bits, true);
		byte* yn = LoadImage(yneg, &width, &height, &bits, true);
		byte* zp = LoadImage(zpos, &width, &height, &bits, true);
		byte* zn = LoadImage(zneg, &width, &height, &bits, true);
	}

	void D3DTextureCube::LoadFromVCross(int32 mips)
	{
		uint srcWidth, srcHeight, bits;
		byte*** cubeTextureData = spnew byte**[mips];
		for (int32 i = 0; i < mips; i++)
			cubeTextureData[i] = spnew byte*[6];

		uint* faceWidths = spnew uint[mips];
		uint* faceHeights = spnew uint[mips];

		for (int32 m = 0; m < mips; m++)
		{
			byte* data = LoadImage(m_Files[m], &srcWidth, &srcHeight, &bits, true);
			uint stride = bits / 8;

			uint face = 0;
			uint faceWidth = srcWidth / 3;
			uint faceHeight = srcHeight / 4;
			faceWidths[m] = faceWidth;
			faceHeights[m] = faceHeight;
			for (uint cy = 0; cy < 4; cy++)
			{
				for (uint cx = 0; cx < 3; cx++)
				{
					if (cy == 0 || cy == 2 || cy == 3)
					{
						if (cx != 1)
							continue;
					}

					cubeTextureData[m][face] = spnew byte[faceWidth * faceHeight * stride];
					uint index = 0;
					for (uint y = 0; y < faceHeight; y++)
					{
						uint offset = y;
						if (face == 5)
							offset = faceHeight - (y + 1);
						uint yp = cy * faceHeight + offset;
						for (uint x = 0; x < faceWidth; x++)
						{
							offset = x;
							if (face == 5)
								offset = faceWidth - (x + 1);
							uint xp = cx * faceWidth + offset;
							cubeTextureData[m][face][(x + y * faceWidth) * stride + 0] = data[(xp + yp * srcWidth) * stride + 0];
							cubeTextureData[m][face][(x + y * faceWidth) * stride + 1] = data[(xp + yp * srcWidth) * stride + 1];
							cubeTextureData[m][face][(x + y * faceWidth) * stride + 2] = data[(xp + yp * srcWidth) * stride + 2];
							if (stride >= 4)
								cubeTextureData[m][face][(x + y * faceWidth) * stride + 3] = data[(xp + yp * srcWidth) * stride + 3];
						}
					}
					face++;
				}
			}
			spdel[] data;
		}

		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = faceWidths[0];
		texDesc.Height = faceHeights[0];
		texDesc.MipLevels = mips;
		texDesc.ArraySize = 6;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.CPUAccessFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
		SMViewDesc.Format = texDesc.Format;
		SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
		SMViewDesc.TextureCube.MostDetailedMip = 0;

		D3D11_SUBRESOURCE_DATA* pData = spnew D3D11_SUBRESOURCE_DATA[6 * mips];

		uint result = 0;
		uint index = 0;
		uint faceOrder[6] = { 3, 1, 0, 4, 2, 5 };
		for (int32 f = 0; f < 6; f++)
		{
			uint fi = faceOrder[f];
			for (int32 m = 0; m < mips; m++)
			{
				pData[index].pSysMem = cubeTextureData[m][fi];
				pData[index].SysMemPitch = faceWidths[m] * 4;
				pData[index].SysMemSlicePitch = faceWidths[m] * faceHeights[m] * 4;
				index++;
			}
		}
		m_Texture = nullptr;
		DXCall(D3DContext::GetDevice()->CreateTexture2D(&texDesc, pData, &m_Texture));
		DXCall(D3DContext::GetDevice()->CreateShaderResourceView(m_Texture, &SMViewDesc, &m_ResourceView));

		spdel[] pData;

		for (int32 m = 0; m < mips; m++)
		{
			for (int32 f = 0; f < 6; f++)
			{
				spdel[] cubeTextureData[m][f];
			}
			spdel[] cubeTextureData[m];
		}
		spdel[] cubeTextureData;

		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		DXCall(D3DContext::GetDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState));
	}

} } }