#pragma once

#include "sp/graphics/Renderer.h"
#include "DXContext.h"

namespace sp { namespace graphics {

	class D3DRenderer : public Renderer
	{
	private:
		static std::vector<ID3D11BlendState*> s_BlendStates;
		static std::vector<ID3D11DepthStencilState*> s_DepthStencilStates;
	private:
		API::D3DContext* m_Context;
		String m_RendererTitle;
	public:
		D3DRenderer();
	protected:
		void InitInternal() override;

		void ClearInternal(uint buffer) override;
		void PresentInternal() override;

		void SetDepthTestingInternal(bool enabled) override;
		void SetBlendInternal(bool enabled) override;
		void SetViewportInternal(uint x, uint y, uint width, uint height) override;

		void SetBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination) override;
		void SetBlendEquationInternal(RendererBlendFunction blendEquation) override;

		const String& GetTitleInternal() const override;
	private:
		void CreateBlendStates();
		void CreateDepthStencilStates();
	};


} }
