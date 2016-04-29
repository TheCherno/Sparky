#pragma once

#include "Mesh.h"
#include "RenderCommand.h"
#include "camera/Camera.h"

#include "LightSetup.h"
#include "postfx/PostEffects.h"

namespace sp { namespace graphics {

	typedef std::vector<RenderCommand> CommandQueue;
	typedef std::vector<RendererUniform> SystemUniformList;

	struct RenderStats
	{
		// rt = render time
		float rtTotal;
		float rtMeshes;
		float rtPostEffects;
	};

	//
	// Base class for all 3D renderers.
	// 
	// Implementations:
	//		- ForwardRenderer.h  (WIP)
	//		- DeferredRenderer.h (WIP)
	//
	class SP_API Renderer3D
	{
	protected:
		uint m_ScreenBufferWidth, m_ScreenBufferHeight; // TODO: Probably make a screen buffer (fb) class

		CommandQueue m_CommandQueue;
		SystemUniformList m_SystemUniforms;
		PostEffects* m_PostEffects;
		bool m_PostEffectsEnabled;

		SP_DEBUG_VAR(RenderStats m_RenderStats);
	public:
		virtual ~Renderer3D() {}

		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void BeginScene(Camera* camera) = 0;
		// TODO: Submit needs to be replaced by some sort of macro
		virtual void Submit(const RenderCommand& command) = 0;
		virtual void SubmitMesh(Mesh* mesh, const maths::mat4& transform) = 0;
		virtual void SubmitLightSetup(const LightSetup& lightSetup) = 0;
		virtual void EndScene() = 0;
		virtual void End() = 0;
		virtual void Present() = 0;

		virtual void SetPostEffects(bool enabled) = 0;
		virtual void PushPostEffectsPass(PostEffectsPass* pass) = 0;
		virtual PostEffectsPass* PopPostEffectsPass() = 0;

		virtual void SetScreenBufferSize(uint width, uint height) { m_ScreenBufferWidth = width; m_ScreenBufferHeight = height; }

		inline bool IsPostEffectsEnabled() const { return m_PostEffectsEnabled; }
		inline const RenderStats& GetRenderStats() const { return m_RenderStats; }
	};

} }