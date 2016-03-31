#pragma once

#include "sp/Common.h"
#include "sp/String.h"

namespace sp { namespace graphics {

	enum SP_API RendererBufferType
	{
		RENDERER_BUFFER_NONE		= 0,
		RENDERER_BUFFER_COLOR		= BIT(0),
		RENDERER_BUFFER_DEPTH		= BIT(1),
		RENDERER_BUFFER_STENCIL		= BIT(2)
	};

	enum class SP_API RendererBlendFunction
	{
		NONE,
		ZERO,
		ONE,
		SOURCE_ALPHA,
		DESTINATION_ALPHA,
		ONE_MINUS_SOURCE_ALPHA
	};

	enum class SP_API RendererBlendEquation
	{
		NONE,
		ADD,
		SUBTRACT
	};

	class SP_API Renderer
	{
	private:
		static Renderer* s_Instance;
	protected:
		virtual void InitInternal() = 0;

		virtual void ClearInternal(uint buffer) = 0;
		virtual void PresentInternal() = 0;

		virtual void SetDepthTestingInternal(bool enabled) = 0;
		virtual void SetBlendInternal(bool enabled) = 0;
		virtual void SetViewportInternal(uint x, uint y, uint width, uint height) = 0;

		virtual void SetBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination) = 0;
		virtual void SetBlendEquationInternal(RendererBlendFunction blendEquation) = 0;

		virtual const String& GetTitleInternal() const = 0;
	public:
		inline static void Clear(uint buffer) { s_Instance->ClearInternal(buffer); }
		inline static void Present() { s_Instance->PresentInternal(); }

		inline static void SetDepthTesting(bool enabled) { s_Instance->SetDepthTestingInternal(enabled); }
		inline static void SetBlend(bool enabled) { s_Instance->SetBlendInternal(enabled); }
		inline static void SetViewport(uint x, uint y, uint width, uint height) { s_Instance->SetViewportInternal(x, y, width, height); }

		inline static void SetBlendFunction(RendererBlendFunction source, RendererBlendFunction destination) { s_Instance->SetBlendFunctionInternal(source, destination); }
		inline static void SetBlendEquation(RendererBlendFunction blendEquation) { s_Instance->SetBlendEquationInternal(blendEquation); }

		inline static const String& GetTitle() { return s_Instance->GetTitleInternal(); }
	public:
		static void Init();
		inline static Renderer* GetRenderer() { return s_Instance; }
	};

} }
