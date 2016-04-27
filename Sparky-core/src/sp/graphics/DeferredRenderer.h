#pragma once

#include "sp/Common.h"
#include "Renderer3D.h"

#include "GBuffer.h"

namespace sp { namespace graphics {

	class DeferredRenderer : public Renderer3D
	{
	private:
		GBuffer* m_GBuffer;
		uint m_Width, m_Height;
		API::Shader* m_GeometryPassShader;
	public:
		DeferredRenderer(uint width, uint height);
		~DeferredRenderer();

		void Init() override;
		void Begin() override;
		void BeginScene(Camera* camera) override;
		void Submit(const RenderCommand& command) override;
		void SubmitMesh(Mesh* mesh, const maths::mat4& transform) override;
		void SubmitLightSetup(const LightSetup& lightSetup);
		void EndScene() override;
		void End() override;
		void Present() override;

		inline const GBuffer& GetGBuffer() const { return *m_GBuffer; }

		void PushPostEffectsPass(PostEffectsPass* pass) override;
		PostEffectsPass* PopPostEffectsPass() override;
	private:
		void SetRequiredUniforms(API::Shader* shader, const std::vector<RendererUniform>& uniforms);
		void SetSystemUniforms(API::Shader* shader);
	};

} }