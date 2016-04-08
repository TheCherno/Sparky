#pragma once

#include "sp/Common.h"
#include "Renderer3D.h"
#include "shaders/Shader.h"
#include "API/TextureDepth.h"

namespace sp { namespace graphics {

	class SP_API ForwardRenderer : public Renderer3D
	{
	private:
		byte* m_VSSystemUniformBuffer;
		uint m_VSSystemUniformBufferSize;
		byte* m_PSSystemUniformBuffer;
		uint m_PSSystemUniformBufferSize;

		std::vector<uint> m_VSSystemUniformBufferOffsets;
		std::vector<uint> m_PSSystemUniformBufferOffsets;

		API::TextureDepth* m_DepthTexture;
		const LightSetup* m_LightSetup;
		bool m_Shadows;
	public:
		ForwardRenderer();
		ForwardRenderer(uint width, uint height);

		void Init() override;
		void Begin() override;
		void BeginScene(Camera* camera) override;
		void Submit(const RenderCommand& command) override;
		void SubmitMesh(Mesh* mesh, const maths::mat4& transform) override;
		void SubmitLightSetup(const LightSetup& lightSetup);
		void EndScene() override;
		void End() override;
		void Present() override;

		inline API::TextureDepth* GetDepthTexture() const { return m_DepthTexture; }
	private:
		void PrepareLights();
		void ShadowPass();
		void SetSystemUniforms(API::Shader* shader);
	};

} }
