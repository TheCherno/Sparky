#pragma once

#include "sp/graphics/API/TextureDepth.h"
#include "DXCommon.h"

namespace sp { namespace graphics { namespace API {

	class D3DTextureDepth : public TextureDepth
	{
	private:
		String m_Name;
		uint m_Width, m_Height;

		ID3D11Texture2D* m_RenderTargetTexture;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11ShaderResourceView* m_ShaderResourceView;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;
		D3D11_VIEWPORT m_Viewport;
		ID3D11SamplerState* m_SamplerState;
		D3D11_SAMPLER_DESC m_SamplerDesc;
	public:
		D3DTextureDepth(uint width, uint height);
		~D3DTextureDepth();

		void Clear() override;
		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void BindForWriting() const override;
		void UnbindForWriting() const override;

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilepath() const override { return m_Name; }
	protected:
		void Init();
	};

} } }
