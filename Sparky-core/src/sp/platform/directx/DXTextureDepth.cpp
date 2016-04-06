#include "sp/sp.h"
#include "DXTextureDepth.h"
#include "DXContext.h"

namespace sp { namespace graphics { namespace API {

	D3DTextureDepth::D3DTextureDepth(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	D3DTextureDepth::~D3DTextureDepth()
	{
		// GLCall(glDeleteTextures(1, &m_Handle));
	}

	void D3DTextureDepth::Init()
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = m_Width;
		textureDesc.Height = m_Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		result = D3DContext::GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_RenderTargetTexture);
		if (FAILED(result))
		{
		}

		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		result = D3DContext::GetDevice()->CreateRenderTargetView(m_RenderTargetTexture, &renderTargetViewDesc, &m_RenderTargetView);
		if (FAILED(result))
		{
		}

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		result = D3DContext::GetDevice()->CreateShaderResourceView(m_RenderTargetTexture, &shaderResourceViewDesc, &m_ShaderResourceView);
		if (FAILED(result))
		{
		}

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = m_Width;
		depthBufferDesc.Height = m_Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = D3DContext::GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
		if (FAILED(result))
		{
		}

		// Initailze the depth stencil view description.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = D3DContext::GetDevice()->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
		if (FAILED(result))
		{
		}

		// Setup the viewport for rendering.
		m_Viewport.Width = (float)m_Width;
		m_Viewport.Height = (float)m_Height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
	}

	void D3DTextureDepth::Clear()
	{
		float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
		D3DContext::GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView, color);
		D3DContext::GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void D3DTextureDepth::Bind(uint slot) const
	{
		D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
		// D3DContext::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);

	}

	void D3DTextureDepth::Unbind(uint slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void D3DTextureDepth::BindForWriting() const
	{
		D3DContext::GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
		D3DContext::GetDeviceContext()->RSSetViewports(1, &m_Viewport);
	}

	void D3DTextureDepth::UnbindForWriting() const
	{
		// TODO: Implement in DXRenderer
		D3DContext::SetRenderTargets(D3DContext::GetBackBuffer(), D3DContext::GetDepthStencilBuffer());
		D3DContext::GetDeviceContext()->RSSetViewports(1, D3DContext::GetViewport());

	}

} } }