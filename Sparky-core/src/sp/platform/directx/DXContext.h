#pragma once

#include "sp/graphics/API/Context.h"
#include "DXCommon.h"

namespace sp { namespace graphics { namespace API {

	class D3DContext : public Context
	{
	public:
		IDXGISwapChain* swapchain;
		ID3D11Device* dev;
		ID3D11DeviceContext* devcon;
		ID3D11RenderTargetView* backbuffer;
	private:
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		uint m_MSAAQuality;
		bool m_MSAAEnabled;
		bool m_DebugLayerEnabled;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		D3D11_VIEWPORT m_ScreenViewport;
		ID3D11Texture2D* m_DepthStencilBuffer;

		ID3D11Debug* m_DebugLayer;

		// Temp
		WindowProperties m_Properties;
	public:
		D3DContext(WindowProperties properties, void* deviceContext);

		void Present();
	private:
		void InitD3D(HWND hWnd);
		void Resize();

		void SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view);

		String GetD3DVersionStringInternal() const;
	public:
		inline static D3DContext* GetContext() { return (D3DContext*)s_Context; }

		inline static IDXGISwapChain* GetSwapChain() { return GetContext()->swapchain; }
		inline static ID3D11Device* GetDevice() { return GetContext()->dev; }
		inline static ID3D11DeviceContext* GetDeviceContext() { return GetContext()->devcon; }
		inline static ID3D11RenderTargetView* GetBackBuffer() { return GetContext()->m_RenderTargetView; }

		inline static ID3D11DepthStencilView* GetDepthStencilBuffer() { return GetContext()->m_DepthStencilView; }

		inline static String GetD3DVersionString() { return Get()->GetD3DVersionStringInternal(); }
	public:
		inline static D3DContext* Get() { return (D3DContext*)s_Context; }
	};

} } }