#pragma once

#include "sp/graphics/API/TextureCube.h"

#include "DXCommon.h"

namespace sp { namespace graphics { namespace API {

	class D3DTextureCube : public TextureCube
	{
	private:
		uint m_Width, m_Height;
		String m_Name;
		String* m_Files;
		uint m_Bits;
		InputFormat m_Format;

		D3D11_TEXTURE2D_DESC m_Desc;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ResourceView;
		ID3D11SamplerState* m_SamplerState;
		D3D11_SAMPLER_DESC m_SamplerDesc;
	public:
		D3DTextureCube(const String& name, const String& filepath);
		D3DTextureCube(const String& name, const String* files);
		D3DTextureCube(const String& name, const String* files, int32 mips, InputFormat format);
		~D3DTextureCube();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot  = 0) const override;

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilepath() const override { return m_Files[0]; }
	private:
		void LoadFromSingleFile();
		void LoadFromMultipleFiles();
		void LoadFromVCross(int32 mips);
	};


} } }
