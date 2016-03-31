#include "sp/sp.h"
#include "ForwardRenderer.h"

#include <GL/glew.h> // TODO: Temp

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
		m_SystemUniforms.clear();
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

		for (uint i = 0; i < m_SystemUniforms.size(); i++)
			command.uniforms.push_back(m_SystemUniforms[i]);

		m_CommandQueue.push_back(command);
	}

	void ForwardRenderer::SubmitLightSetup(const LightSetup& lightSetup)
	{
		auto lights = lightSetup.GetLights();
		SP_ASSERT(lights.size() <= 1, "Only one light is supported at the moment!");
		for (uint i = 0; i < lights.size(); i++)
		{
			Light* light = lights[i];
			m_SystemUniforms.push_back({ "u_LightPosition",  (byte*)&light->position });
			m_SystemUniforms.push_back({ "u_LightAttenuation",  (byte*)&light->attenuation });
			m_SystemUniforms.push_back({ "u_LightColor",  (byte*)&light->color });
		}
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

	void ForwardRenderer::SetSystemUniforms(Shader* shader)
	{
		const SystemUniformList& uniforms = m_SystemUniforms;
		for (uint i = 0; i < uniforms.size(); i++)
			shader->SetUniform(uniforms[i].uniform, uniforms[i].value);
	}

	void ForwardRenderer::Present()
	{
		// TODO: Shader binding, texture sorting, visibility testing, etc.

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));

		for (uint i = 0; i < m_CommandQueue.size(); i++)
		{
			const RenderCommand& command = m_CommandQueue[i];
			command.mesh->GetMaterialInstance()->GetMaterial()->GetShader()->Bind();
			SetRequiredUniforms(command.mesh->GetMaterialInstance()->GetMaterial()->GetShader(), command.uniforms);
			//SetSystemUniforms(command.mesh->GetMaterialInstance()->GetMaterial()->GetShader());
			command.mesh->Render(*this);
		}
	}

} }