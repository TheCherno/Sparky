#pragma once

#include "sp/graphics/Renderer.h"
#include "GLContext.h"
#include "GLCommon.h"

namespace sp { namespace graphics {

	class GLRenderer : public Renderer
	{
	private:
		API::GLContext* m_Context;
		String m_RendererTitle;
	public:
		GLRenderer();
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
	public:
		static uint SPRendererBufferToGL(uint buffer);
		static uint SPRendererBlendFunctionToGL(RendererBlendFunction function);
	};

} }
