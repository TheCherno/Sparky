#include "sp/sp.h"
#include "ForwardRenderer.h"

namespace sp { namespace graphics {

	ForwardRenderer::ForwardRenderer()
	{

	}

	void ForwardRenderer::Init()
	{

	}

	void ForwardRenderer::Begin()
	{
		m_CommandQueue.clear();
	}

	void ForwardRenderer::Submit(const RenderCommand& command)
	{
		m_CommandQueue.push_back(command);
	}

	void ForwardRenderer::SubmitMesh(Camera* camera, Mesh* mesh, const maths::mat4& transform)
	{
		RendererUniform pr_matrix;
		pr_matrix.uniform = SHADER_UNIFORM_PROJECTION_MATRIX_NAME;
		pr_matrix.value = (byte*)&camera->GetProjectionMatrix();

		RendererUniform vw_matrix;
		vw_matrix.uniform = SHADER_UNIFORM_VIEW_MATRIX_NAME;
		vw_matrix.value = (byte*)&camera->GetViewMatrix();

		RendererUniform ml_matrix;
		ml_matrix.uniform = SHADER_UNIFORM_MODEL_MATRIX_NAME;
		ml_matrix.value = (byte*)&transform;

		RenderCommand command;
		command.mesh = mesh;
		command.uniforms.push_back(pr_matrix);
		command.uniforms.push_back(vw_matrix);
		command.uniforms.push_back(ml_matrix);
		m_CommandQueue.push_back(command);
	}

	void ForwardRenderer::End()
	{
		// TODO: Batching and sorting
	}

	void ForwardRenderer::SetRequiredUniforms(Shader* shader, const std::vector<RendererUniform>& uniforms)
	{
		for (uint i = 0; i < uniforms.size(); i++)
			shader->SetUniform(uniforms[i].uniform, uniforms[i].value);
	}

	void ForwardRenderer::Present()
	{
		// TODO: Shader binding, texture sorting, visibility testing, etc.

		for (uint i = 0; i < m_CommandQueue.size(); i++)
		{
			const RenderCommand& command = m_CommandQueue[i];
			command.mesh->GetMaterialInstance()->GetMaterial()->GetShader()->Bind();
			SetRequiredUniforms(command.mesh->GetMaterialInstance()->GetMaterial()->GetShader(), command.uniforms);
			command.mesh->Render(*this);
		}
	}

} }