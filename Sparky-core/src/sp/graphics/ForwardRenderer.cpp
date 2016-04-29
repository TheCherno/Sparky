#include "sp/sp.h"
#include "ForwardRenderer.h"

#include "sp/app/Application.h"
#include "sp/graphics/Renderer.h"
#include "MeshFactory.h"
#include "shaders/ShaderFactory.h"

#include "sp/debug/DebugLayer.h"
#include "sp/utils/Timer.h"

namespace sp { namespace graphics {

	using namespace maths;

	enum VSSystemUniformIndices : int32
	{
		VSSystemUniformIndex_ProjectionMatrix	= 0,
		VSSystemUniformIndex_ViewMatrix			= 1,
		VSSystemUniformIndex_ModelMatrix		= 2,
		VSSystemUniformIndex_CameraPosition		= 3,
		VSSystemUniformIndex_Size
	};

	enum PSSystemUniformIndices : int32
	{
		PSSystemUniformIndex_Lights = 0,
		PSSystemUniformIndex_Size
	};

	ForwardRenderer::ForwardRenderer()
	{
		SetScreenBufferSize(Application::GetApplication().GetWindowWidth(), Application::GetApplication().GetWindowHeight());
		Init();
	}

	ForwardRenderer::ForwardRenderer(uint width, uint height)
	{
		SetScreenBufferSize(width, height);
		Init();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		spdel m_PostEffects;
		spdel m_PresentationMaterial;
	}

	void ForwardRenderer::Init()
	{
		m_PostEffectsEnabled = false;
		m_PostEffects = spnew PostEffects();

		SP_DEBUG_BLOCK(memset(&m_RenderStats, 0, sizeof(RenderStats)));

		m_CommandQueue.reserve(1000);

		//
		// Vertex shader system uniforms
		//
		m_VSSystemUniformBufferSize = sizeof(mat4) + sizeof(mat4) + sizeof(vec3) + sizeof(mat4);
		m_VSSystemUniformBuffer = spnew byte[m_VSSystemUniformBufferSize];
		memset(m_VSSystemUniformBuffer, 0, m_VSSystemUniformBufferSize);
		m_VSSystemUniformBufferOffsets.resize(VSSystemUniformIndex_Size);

		// Per Scene System Uniforms
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ProjectionMatrix] = 0;
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ViewMatrix] = m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ProjectionMatrix] + sizeof(mat4);
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ModelMatrix] = m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ViewMatrix] + sizeof(mat4);

		// Per Mesh System Uniforms
		// Note: Model Matrix should be here instead of camera position; this will get sorted when it gets moved to a separate buffer
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_CameraPosition] = m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ModelMatrix] + sizeof(mat4);

		//
		// Pixel/fragment shader system uniforms
		//
		m_PSSystemUniformBufferSize = sizeof(Light);
		m_PSSystemUniformBuffer = spnew byte[m_PSSystemUniformBufferSize];
		memset(m_PSSystemUniformBuffer, 0, m_PSSystemUniformBufferSize);
		m_PSSystemUniformBufferOffsets.resize(PSSystemUniformIndex_Size);

		// Per Scene System Uniforms
		m_PSSystemUniformBufferOffsets[PSSystemUniformIndex_Lights] = 0;

		m_RenderBuffer = Framebuffer2D::Create(m_ScreenBufferWidth, m_ScreenBufferHeight);
		m_ScreenBuffer = Framebuffer2D::Create(m_ScreenBufferWidth, m_ScreenBufferHeight);
		m_PresentationMaterial = spnew Material(ShaderFactory::SimpleShader());
		m_PresentationMaterial->SetTexture("u_Texture", m_ScreenBuffer->GetTexture());

		m_PresentationMaterial->Bind();
		m_ScreenQuad = MeshFactory::CreateQuad(-1, -1, 2, 2, spnew MaterialInstance(m_PresentationMaterial));
	}

	void ForwardRenderer::Begin()
	{
		Renderer::SetViewport(0, 0, m_ScreenBufferWidth, m_ScreenBufferHeight);

		m_CommandQueue.clear();
		m_SystemUniforms.clear();
	}

	void ForwardRenderer::BeginScene(Camera* camera)
	{
		memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ProjectionMatrix], &camera->GetProjectionMatrix(), sizeof(mat4));
		memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ViewMatrix], &camera->GetViewMatrix(), sizeof(mat4));
		memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_CameraPosition], &camera->GetPosition(), sizeof(vec3));
	}

	void ForwardRenderer::Submit(const RenderCommand& command)
	{
		m_CommandQueue.push_back(command);
	}

	void ForwardRenderer::SubmitMesh(Mesh* mesh, const maths::mat4& transform)
	{
		RenderCommand command;
		command.mesh = mesh;
		command.transform = transform; // TODO: Come up with a better solution to per-mesh system uniforms
		command.shader = mesh->GetMaterialInstance()->GetMaterial()->GetShader();
		Submit(command);
	}

	void ForwardRenderer::SubmitLightSetup(const LightSetup& lightSetup)
	{
		const auto& lights = lightSetup.GetLights();
		SP_ASSERT(lights.size() <= 1, "Only one light is supported at the moment!");
		for (uint i = 0; i < lights.size(); i++)
			memcpy(m_PSSystemUniformBuffer + m_PSSystemUniformBufferOffsets[PSSystemUniformIndex_Lights], lights[i], sizeof(Light));
	}

	void ForwardRenderer::EndScene()
	{
	}

	void ForwardRenderer::End()
	{
		// TODO: Batching and sorting
	}

	void ForwardRenderer::SetSystemUniforms(API::Shader* shader)
	{
		// TODO: Set per-mesh buffer to slot 1
		shader->SetVSSystemUniformBuffer(m_VSSystemUniformBuffer, m_VSSystemUniformBufferSize, 0);
		shader->SetPSSystemUniformBuffer(m_PSSystemUniformBuffer, m_PSSystemUniformBufferSize, 0);
	}

	void ForwardRenderer::Present()
	{
		SP_DEBUG_BLOCK(Timer frPresentTimer);

		// TODO: Shader binding, texture sorting, visibility testing, etc.
		m_RenderBuffer->Clear();
		m_RenderBuffer->Bind();
		SP_TIMER_BLOCK(RenderMeshes(), m_RenderStats.rtMeshes);
		m_RenderBuffer->Unbind();

		SP_TIMER_BLOCK(RenderPostEffects(), m_RenderStats.rtPostEffects);

		static mat4 proj = mat4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		m_PresentationMaterial->GetShader()->SetVSSystemUniformBuffer((byte*)&proj, sizeof(mat4));
		m_PresentationMaterial->Bind();
		m_ScreenQuad->Render(*this);
		m_PresentationMaterial->Unbind();

		SP_DEBUG_BLOCK(m_RenderStats.rtTotal = frPresentTimer.ElapsedMillis());
	}

	void ForwardRenderer::RenderMeshes()
	{
		for (uint i = 0; i < m_CommandQueue.size(); i++)
		{
			RenderCommand& command = m_CommandQueue[i];
			MaterialInstance* material = command.mesh->GetMaterialInstance();
			int materialRenderFlags = material->GetRenderFlags();
			Renderer::SetDepthTesting((materialRenderFlags & (int)Material::RenderFlags::DISABLE_DEPTH_TEST) == 0);
			memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ModelMatrix], &command.transform, sizeof(mat4));
			SetSystemUniforms(command.shader);
			material->Bind();
			command.mesh->Render(*this);
			material->Unbind();

#if defined(SP_DEBUG) && 0
			uint j;
			for (j = 0; j < command.uniforms.size(); j++)
			{
				if (command.uniforms[j].uniform == "ml_matrix")
				{
					command.mesh->DebugRender(*(maths::mat4*)command.uniforms[j].value);
					break;
				}
			}
#endif
		}
	}

	void ForwardRenderer::RenderPostEffects()
	{
		if (!m_PostEffectsEnabled)
			return;

		for (PostEffectsPass* pass : m_PostEffects->GetPasses())
		{
			Material* material = pass->GetMaterial();
			material->SetTexture("u_ScreenTexture", m_RenderBuffer->GetTexture());

			mat4 proj = mat4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
			material->GetShader()->SetVSSystemUniformBuffer((byte*)&proj, sizeof(mat4));

			m_ScreenBuffer->Bind();
			material->Bind();
			m_ScreenQuad->Render(*this);
			material->Unbind();
			m_ScreenBuffer->Unbind();
		}
	}

	void ForwardRenderer::SetPostEffects(bool enabled)
	{
		if (enabled)
			m_PresentationMaterial->SetTexture("u_Texture", m_ScreenBuffer->GetTexture());
		else
			m_PresentationMaterial->SetTexture("u_Texture", m_RenderBuffer->GetTexture());

		m_PostEffectsEnabled = enabled;
	}

	void ForwardRenderer::PushPostEffectsPass(PostEffectsPass* pass)
	{
		m_PostEffects->Push(pass);
	}

	PostEffectsPass* ForwardRenderer::PopPostEffectsPass()
	{
		return m_PostEffects->Pop();
	}

} }