#include "sp/sp.h"
#include "DXContext.h"

#include <Windows.h>
#include <d3d11.h>

#include "sp/utils/Log.h"

namespace sp { namespace graphics { namespace API {

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;		\
	}

	D3DContext::D3DContext(WindowProperties properties, void* deviceContext)
		: m_DebugLayerEnabled(true)
	{
		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilBuffer = nullptr;

		m_Properties = properties;
		InitD3D((HWND)deviceContext);
	}

	void D3DContext::InitD3D(HWND hWnd)
	{
		m_MSAAEnabled = true;

		HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, m_DebugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, NULL, D3D11_SDK_VERSION, &dev, &m_D3DFeatureLevel, &devcon);
		dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);
		// assert(m_MSAAQuality > 0);

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = m_Properties.width;
		scd.BufferDesc.Height = m_Properties.height;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		scd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		scd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 3;
		scd.OutputWindow = hWnd;
		scd.Windowed = !m_Properties.fullscreen;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIDevice* dxgiDevice = 0;
		IDXGIAdapter* dxgiAdapter = 0;
		IDXGIFactory* dxgiFactory = 0;

		dev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		dxgiFactory->CreateSwapChain(dev, &scd, &swapchain);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		if (m_DebugLayerEnabled)
		{
			dev->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_DebugLayer));
			m_DebugLayer->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);

			ID3D11InfoQueue* infoQueue;
			dev->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));
			D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = 1;
			filter.DenyList.pIDList = hide;
			infoQueue->AddStorageFilterEntries(&filter);
		}

		Resize();
	}

	void D3DContext::Resize()
	{
		uint width = m_Properties.width;
		uint height = m_Properties.height;

		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);

		swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		ID3D11Texture2D *backBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		dev->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		backBuffer->Release();

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		dev->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer);
		dev->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView);
		SetRenderTargets(m_RenderTargetView, m_DepthStencilView);

		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		devcon->RSSetViewports(1, &m_ScreenViewport);

		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		ID3D11RasterizerState* rs;
		dev->CreateRasterizerState(&rasterDesc, &rs);
		devcon->RSSetState(rs);
		ReleaseCOM(rs);
	}

	void D3DContext::SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view)
	{
		devcon->OMSetRenderTargets(1, &target, view);
	}

	void D3DContext::Present()
	{
		swapchain->Present(m_Properties.vsync, 0);
	}

	String D3DContext::GetD3DVersionStringInternal() const
	{
		switch (m_D3DFeatureLevel)
		{
			case D3D_FEATURE_LEVEL_11_0:	return "11";
			case D3D_FEATURE_LEVEL_10_1:	return "10.1";
			case D3D_FEATURE_LEVEL_10_0:	return "10";
		}
		return "Unknown";
	}

} } }