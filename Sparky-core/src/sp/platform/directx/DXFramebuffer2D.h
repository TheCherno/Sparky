#pragma once

#include "sp/graphics/API/Framebuffer2D.h"

#include "DXCommon.h"
#include "DXTexture2D.h"

namespace sp { namespace graphics {

	class D3DFramebuffer2D : public Framebuffer2D
	{
	private:
		uint m_Width, m_Height;
		maths::vec4 m_ClearColor;

		API::D3DTexture2D* m_Texture;

		ID3D11Texture2D* m_RenderTargetTexture;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11ShaderResourceView* m_ShaderResourceView;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;
		D3D11_VIEWPORT m_Viewport;
	public:
		D3DFramebuffer2D(uint width, uint height);
		~D3DFramebuffer2D();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		inline uint GetWidth() const override { return m_Width; }
		inline uint GetHeight() const override { return m_Height; }

		inline API::Texture* GetTexture() const override { return m_Texture; }
		inline API::Texture2D* GetTexture2D() const override { return m_Texture; }
		inline void SetClearColor(const maths::vec4& color) override { m_ClearColor = color; }
	private:
		void Init();
	};

} }
