#pragma once

#include "sp/graphics/API/Texture2D.h"

#include "DXCommon.h"

namespace sp { namespace graphics { namespace API {

	class D3DTexture2D : public Texture2D
	{
	private:
		String m_Name;
		String m_FileName;
		uint m_Handle;
		uint m_Width, m_Height;
		uint m_BitsPerPixel;
		TextureParameters m_Parameters;
		TextureLoadOptions m_LoadOptions;

		D3D11_TEXTURE2D_DESC m_Desc;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ResourceView;
		ID3D11SamplerState* m_SamplerState;
		D3D11_SAMPLER_DESC m_SamplerDesc;
	public:
		D3DTexture2D(uint width, uint height, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		D3DTexture2D(uint width, uint height, uint color, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		D3DTexture2D(const String& name, const String& filename, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());

		~D3DTexture2D();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		virtual void SetData(const void* pixels) override;
		virtual void SetData(uint color) override {}

		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight() const { return m_Height; }

		inline const String& GetName() const { return m_Name; }
		inline const String& GetFilepath() const { return m_FileName; }
	public:
		static DXGI_FORMAT SPTextureFormatToD3D(TextureFormat format);
		static uint SPTextureFilterToD3D(TextureFilter filter);
		static uint SPTextureWrapToD3D(TextureWrap wrap);

		static D3DTexture2D* FromFile(const String& filepath);
	private:
		void Load();
	};

} } }
