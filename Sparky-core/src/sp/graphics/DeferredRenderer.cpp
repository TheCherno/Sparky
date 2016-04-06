#include "sp/sp.h"
#include "DeferredRenderer.h"

#include "sp/graphics/shaders/ShaderFactory.h"

namespace sp { namespace graphics {

#if 0
	DeferredRenderer::DeferredRenderer(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	DeferredRenderer::~DeferredRenderer()
	{
		spdel m_GBuffer;
		spdel m_GeometryPassShader;
	}

	void DeferredRenderer::Init()
	{
		m_GBuffer = new GBuffer(m_Width, m_Height);
		m_GeometryPassShader = ShaderFactory::GeometryPassShader();
	}

	void DeferredRenderer::Begin()
	{
		m_CommandQueue.clear();
		m_SystemUniforms.clear();
	}

	void DeferredRenderer::BeginScene(Camera* camera)
	{
	}

	void DeferredRenderer::Submit(const RenderCommand& command)
	{
		m_CommandQueue.push_back(command);
	}

	void DeferredRenderer::SubmitMesh(Mesh* mesh, const maths::mat4& transform)
	{
		// Not implemented
		SP_ASSERT(false);
	}

	void DeferredRenderer::SubmitLightSetup(const LightSetup& lightSetup)
	{
		auto lights = lightSetup.GetLights();
		SP_ASSERT(lights.size() <= 1, "Only one light is supported at the moment!");
		for (uint i = 0; i < lights.size(); i++)
			m_SystemUniforms.push_back({ "u_Light",  (byte*)lights[i] });
	}

	void DeferredRenderer::EndScene()
	{
	}

	void DeferredRenderer::End()
	{
	}

	void DeferredRenderer::SetRequiredUniforms(API::Shader* shader, const std::vector<RendererUniform>& uniforms)
	{
	// for (uint i = 0; i < uniforms.size(); i++)
	// 	shader->SetUniform(uniforms[i].uniform, uniforms[i].value);
	}

	void DeferredRenderer::SetSystemUniforms(API::Shader* shader)
	{
		const SystemUniformList& uniforms = m_SystemUniforms;
		// for (uint i = 0; i < uniforms.size(); i++)
		// 	shader->SetUniform(uniforms[i].uniform, uniforms[i].value);
	}

	void DeferredRenderer::Present()
	{
		// TODO: Setup renderer state properly
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));



		// Geometry pass ------------------------------------------------
		GLCall(glDisable(GL_BLEND));
		m_GeometryPassShader->Bind();
		m_GBuffer->Bind(1);
		API::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (uint i = 0; i < m_CommandQueue.size(); i++)
		{
			m_GeometryPassShader->Bind();
			const RenderCommand& command = m_CommandQueue[i];
			SetRequiredUniforms(m_GeometryPassShader, command.uniforms);
			command.mesh->Render(*this);
		}
// 		GLCall(glDepthMask(GL_FALSE));
// 		GLCall(glDisable(GL_DEPTH_TEST));
		// ---------------------------------------------------------------


		// Begin Light Passes --------------------------------------------
// 		GLCall(glEnable(GL_BLEND));
// 		GLCall(glBlendEquation(GL_FUNC_ADD));
// 		GLCall(glBlendFunc(GL_ONE, GL_ONE));
// 		m_GBuffer->Bind(0);
// 		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		// ---------------------------------------------------------------

		// Light Pass ----------------------------------------------------

		// ---------------------------------------------------------------



		// ---------------------------------------------------------------


		// ---------------------------------------------------------------
		API::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
 		API::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		m_GBuffer->Bind(0);
  		uint halfWidth = m_Width / 2;
  		uint halfHeight = m_Height / 2;
  
  		m_GBuffer->SetReadBuffer(GBuffer::TextureType::Position);
  		API::BlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
  
  		m_GBuffer->SetReadBuffer(GBuffer::TextureType::Diffuse);
  		API::BlitFramebuffer(0, 0, m_Width, m_Height, 0, halfHeight, halfWidth, m_Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
  
  		m_GBuffer->SetReadBuffer(GBuffer::TextureType::Normal);
  		API::BlitFramebuffer(0, 0, m_Width, m_Height, halfWidth, halfHeight, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
  
  		m_GBuffer->SetReadBuffer(GBuffer::TextureType::TextureCoord);
  		API::BlitFramebuffer(0, 0, m_Width, m_Height, halfWidth, 0, m_Width, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		/*for (uint i = 0; i < m_CommandQueue.size(); i++)
		{
			const RenderCommand& command = m_CommandQueue[i];
			command.shader->Bind();
			SetRequiredUniforms(command.shader, command.uniforms);
			SetSystemUniforms(command.shader);
			command.mesh->Render(*this);
		}*/
	}

#endif

} }