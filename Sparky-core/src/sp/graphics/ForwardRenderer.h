#pragma once

#include "Renderer3D.h"

namespace sp { namespace graphics {

	class ForwardRenderer : public Renderer3D
	{
	private:

	public:
		ForwardRenderer();
		void Init() override;
		void Begin() override;
		void Submit(const RenderCommand& command) override;
		void SubmitMesh(Camera* camera, Mesh* mesh, const maths::mat4& transform) override;
		void End() override;
		void Present() override;
	private:
		void SetRequiredUniforms(Shader* shader, const std::vector<RendererUniform>& uniforms);
	};

} }
