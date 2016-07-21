#include "sp/sp.h"
#include "BatchRenderer2D.h"

#include "shaders/ShaderFactory.h"
#include "MeshFactory.h"

#include "API/VertexBuffer.h"
#include "API/VertexArray.h"
#include "API/BufferLayout.h"

#include "sp/utils/Log.h"

#include "Renderer.h"
#include "sp/debug/DebugMenu.h"

#include <freetype-gl/freetype-gl.h>

namespace sp { namespace graphics {

	using namespace maths;

	const uint g_RequiredSystemUniformsCount = 2;
	const String g_RequiredSystemUniforms[g_RequiredSystemUniformsCount] =
	{
		"sys_ProjectionMatrix",
		"sys_ViewMatrix"
	};

	const uint sys_ProjectionMatrixIndex = 0;
	const uint sys_ViewMatrixIndex = 1;

	BatchRenderer2D::BatchRenderer2D(uint width, uint height)
		: m_IndexCount(0), m_ScreenSize(tvec2<uint>(width, height)), m_ViewportSize(tvec2<uint>(width, height))
	{
		Init();
	}

	BatchRenderer2D::BatchRenderer2D(const tvec2<uint>& screenSize)
		: m_IndexCount(0), m_ScreenSize(screenSize), m_ViewportSize(screenSize)
	{
		Init();
	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		spdel m_IndexBuffer;
		spdel m_VertexArray;
		spdel m_ScreenQuad;
	}

	void BatchRenderer2D::Init()
	{
		m_PostEffectsEnabled = false;

		m_SystemUniforms.resize(g_RequiredSystemUniformsCount);

		m_Shader = ShaderFactory::BatchRendererShader();
		const API::ShaderUniformBufferList& vssu = m_Shader->GetVSSystemUniforms();
		SP_ASSERT(vssu.size());
		for (uint i = 0; i < vssu.size(); i++)
		{
			API::ShaderUniformBufferDeclaration* ub = vssu[i];
			UniformBuffer buffer(spnew byte[ub->GetSize()], ub->GetSize());
			m_SystemUniformBuffers.push_back(buffer);
			for (API::ShaderUniformDeclaration* decl : ub->GetUniformDeclarations())
			{
				for (uint j = 0; j < g_RequiredSystemUniformsCount; j++)
				{
					if (decl->GetName() == g_RequiredSystemUniforms[j])
						m_SystemUniforms[j] = BR2DSystemUniform(buffer, decl->GetOffset());
				}
			}
		}

		SetCamera(spnew Camera(mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)));
		
		m_Shader->Bind();

		API::VertexBuffer* buffer = API::VertexBuffer::Create(API::BufferUsage::DYNAMIC);
		buffer->Resize(RENDERER_BUFFER_SIZE);

		API::BufferLayout layout;
		layout.Push<vec3>("POSITION"); // Position
		layout.Push<vec2>("TEXCOORD"); // UV
		layout.Push<vec2>("MASKUV"); // Mask UV
		layout.Push<float>("ID"); // Texture Index
		layout.Push<float>("MASKID"); // Mask Index
		layout.Push<byte>("COLOR", 4, true); // Color
		buffer->SetLayout(layout);

		m_VertexArray = API::VertexArray::Create();
		m_VertexArray->PushBuffer(buffer);

		uint* indices = new uint[RENDERER_INDICES_SIZE];

		int32 offset = 0;
		for (int32 i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IndexBuffer = API::IndexBuffer::Create(indices, RENDERER_INDICES_SIZE);
		m_VertexArray->Unbind();

		// Setup Framebuffer

#if 0
		m_Framebuffer = Framebuffer2D::Create(m_ViewportSize.x, m_ViewportSize.y);
		m_FramebufferMaterial = new Material(ShaderFactory::SimpleShader());
		m_FramebufferMaterial->SetUniform("pr_matrix", maths::mat4::Orthographic(0, (float)m_ScreenSize.x, (float)m_ScreenSize.y, 0, -1.0f, 1.0f));
		m_FramebufferMaterial->SetTexture("u_Texture", m_Framebuffer->GetTexture());
		m_ScreenQuad = MeshFactory::CreateQuad(0, 0, (float)m_ScreenSize.x, (float)m_ScreenSize.y);

		m_PostEffects = new PostEffects();
		m_PostEffectsBuffer = Framebuffer2D::Create(m_ViewportSize.x, m_ViewportSize.y);
#endif

		debug::DebugMenu::Add(String("Renderer2D/Post Effects"), &s_PostEffectsEnabled);
		debug::DebugMenu::Add(String("Renderer2D/Mask"), &s_MaskEnabled);
	}

	float BatchRenderer2D::SubmitTexture(API::Texture* texture)
	{
#if 0
		if (!textureID)
			SP_WARN("Invalid texture ID submitted!");
#endif
		float result = 0.0f;
		bool found = false;
		for (uint i = 0; i < m_Textures.size(); i++)
		{
			if (m_Textures[i] == texture)
			{
				result = (float)(i + 1);
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (m_Textures.size() >= RENDERER_MAX_TEXTURES)
			{
				End();
				Present();
				Begin();
			}
			m_Textures.push_back(texture);
			result = (float)(m_Textures.size());
		}
		return result;
	}

	void BatchRenderer2D::SetCamera(Camera* camera)
	{
		m_Camera = camera;

		memcpy(m_SystemUniforms[sys_ProjectionMatrixIndex].buffer.buffer + m_SystemUniforms[sys_ProjectionMatrixIndex].offset, &camera->GetProjectionMatrix(), sizeof(mat4));
		memcpy(m_SystemUniforms[sys_ViewMatrixIndex].buffer.buffer + m_SystemUniforms[sys_ViewMatrixIndex].offset, &camera->GetViewMatrix(), sizeof(mat4));
	}

	void BatchRenderer2D::Begin()
	{
		if (m_Target == RenderTarget::BUFFER)
		{
			SP_ASSERT(false); // Currently Unsupported
#if 0
			if (m_ViewportSize != m_Framebuffer->GetSize())
			{
				delete m_Framebuffer;
				m_Framebuffer = new API::Framebuffer2D(m_ViewportSize);
				
				if (m_PostEffectsEnabled)
				{
					delete m_PostEffectsBuffer;
					m_PostEffectsBuffer = new API::Framebuffer2D(m_ViewportSize);
				}
			}

			if (m_PostEffectsEnabled)
			{
				m_PostEffectsBuffer->Bind();
				m_PostEffectsBuffer->Clear();
			}

			m_Framebuffer->Bind();
			m_Framebuffer->Clear(); // TODO: Clear somewhere else, since this basically limits to one draw call
			Renderer::SetBlendFunction(RendererBlendFunction::ONE, RendererBlendFunction::ZERO);
#endif
		}
		else
		{
			// GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer));
			Renderer::SetViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);
		}
		m_VertexArray->Bind();
		m_Buffer = m_VertexArray->GetBuffer()->GetPointer<VertexData>();
	}

	void BatchRenderer2D::Submit(const Renderable2D* renderable)
	{
		if (!renderable->IsVisible())
			return;

		const Rectangle& bounds = renderable->GetBounds();
		const vec3 min = bounds.GetMinimumBound();
		const vec3 max = bounds.GetMaximumBound();

		const uint color = renderable->GetColor();
		const std::vector<vec2>& uv = renderable->GetUVs();
		const API::Texture* texture = renderable->GetTexture();

		float textureSlot = 0.0f;
		if (texture)
			textureSlot = SubmitTexture(renderable->GetTexture());

		mat4 maskTransform = mat4::Identity();
		float mid = m_Mask ? SubmitTexture(m_Mask->texture) : 0.0f;
		float ms = 0.0f;

		if (s_MaskEnabled && m_Mask != nullptr)
		{
			maskTransform = mat4::Invert(m_Mask->transform);
			ms = SubmitTexture(m_Mask->texture);
		}

		vec3 vertex = *m_TransformationBack * min;
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[0];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(max.x, min.y);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[1];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * max;
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[2];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(min.x, max.y);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::DrawLine(float x0, float y0, float x1, float y1, uint color, float thickness)
	{
		const std::vector<vec2>& uv = Renderable2D::GetDefaultUVs();
		float ts = 0.0f;
		mat4 maskTransform = mat4::Identity();
		float mid = m_Mask ? SubmitTexture(m_Mask->texture) : 0.0f;

		float ms = 0.0f;
		if (s_MaskEnabled && m_Mask != nullptr)
		{
			maskTransform = mat4::Invert(m_Mask->transform);
			ms = SubmitTexture(m_Mask->texture);
		}

		vec2 normal = vec2(y1 - y0, -(x1 - x0)).Normalise() * thickness;

		vec3 vertex = *m_TransformationBack * vec3(x0 + normal.x, y0 + normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[0];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(x1 + normal.x, y1 + normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[1];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(x1 - normal.x, y1 - normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[2];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(x0 - normal.x, y0 - normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::DrawLine(const maths::vec2& start, const maths::vec2& end, uint color, float thickness)
	{
		DrawLine(start.x, start.y, end.x, end.y, color, thickness);
	}

	void BatchRenderer2D::DrawRect(float x, float y, float width, float height, uint color)
	{
		DrawLine(x, y, x + width, y, color);
		DrawLine(x + width, y, x + width, y + height, color);
		DrawLine(x + width, y + height, x, y + height, color);
		DrawLine(x, y + height, x, y, color);
	}

	void BatchRenderer2D::DrawRect(const maths::vec2& position, const maths::vec2& size, uint color)
	{
		DrawRect(position.x, position.y, size.x, size.y, color);
	}

	void BatchRenderer2D::DrawRect(const Rectangle& rectangle, uint color)
	{
		DrawRect(rectangle.GetMinimumBound(), rectangle.size * 2.0f, color);
	}

	void BatchRenderer2D::DrawString(const String& text, const maths::vec2& position, const Font& font, uint color)
	{
		using namespace ftgl;

		API::Texture2D* texture = font.GetTexture();
		SP_ASSERT(texture);
		float ts = SubmitTexture(texture);

		const vec2& scale = font.GetScale(); // FontManager::GetScale();

		float x = position.x;

		texture_font_t* ftFont = font.GetFTFont();

		for (uint i = 0; i < text.length(); i++)
		{
			char c = text[i];
			texture_glyph_t* glyph = texture_font_get_glyph(ftFont, c);
			if (glyph)
			{
				if (i > 0)
				{
					float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
					x += kerning / scale.x;
				}

				float x0 = x + glyph->offset_x / scale.x;
				float y0 = position.y + glyph->offset_y / scale.y;
				float x1 = x0 + glyph->width / scale.x;
				float y1 = y0 - glyph->height / scale.y;

				float u0 = glyph->s0;
				float v0 = glyph->t0;
				float u1 = glyph->s1;
				float v1 = glyph->t1;

				m_Buffer->vertex = *m_TransformationBack * vec3(x0, y0, 0);
				m_Buffer->uv = vec2(u0, v0);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * vec3(x0, y1, 0);
				m_Buffer->uv = vec2(u0, v1);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * vec3(x1, y1, 0);
				m_Buffer->uv = vec2(u1, v1);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * vec3(x1, y0, 0);
				m_Buffer->uv = vec2(u1, v0);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_IndexCount += 6;

				x += glyph->advance_x / scale.x;
			}
		}
	}

	void BatchRenderer2D::FillRect(float x, float y, float width, float height, uint color)
	{
		vec3 position(x, y, 0.0f);
		vec2 size(width, height);
		const std::vector<vec2>& uv = Renderable2D::GetDefaultUVs();
		float ts = 0.0f;
		mat4 maskTransform = mat4::Identity();
		float mid = m_Mask ? SubmitTexture(m_Mask->texture) : 0.0f;

		float ms = 0.0f;
		if (s_MaskEnabled && m_Mask != nullptr)
		{
			maskTransform = mat4::Invert(m_Mask->transform);
			ms = SubmitTexture(m_Mask->texture);
		}

		vec3 vertex = *m_TransformationBack * position;
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[0];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(position.x + size.x, position.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[1];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[2];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * vec3(position.x, position.y + size.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::FillRect(const maths::vec2& position, const maths::vec2& size, uint color)
	{
		FillRect(position.x, position.y, size.x, size.y, color);
	}

	void BatchRenderer2D::FillRect(const Rectangle& rectangle, uint color)
	{
		FillRect(rectangle.GetMinimumBound(), rectangle.size * 2.0f, color);
	}

	void BatchRenderer2D::End()
	{
		m_VertexArray->GetBuffer()->ReleasePointer();
		m_VertexArray->Unbind();
	}

	void BatchRenderer2D::Present()
	{
		Renderer::SetDepthTesting(false);

		m_Shader->Bind();
		for (uint i = 0; i < m_SystemUniformBuffers.size(); i++)
			m_Shader->SetVSSystemUniformBuffer(m_SystemUniformBuffers[i].buffer, m_SystemUniformBuffers[i].size, i);

		for (uint i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Bind(i);

		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		m_VertexArray->Draw(m_IndexCount);
		m_IndexBuffer->Unbind();
		m_VertexArray->Unbind();

		for (uint i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Unbind(i);

		m_IndexCount = 0;
		m_Textures.clear();
		
		if (m_Target == RenderTarget::BUFFER)
		{
			SP_ASSERT(false); // Currently unsupported
#if 0
			// Post Effects pass should go here!
			if (s_PostEffectsEnabled && m_PostEffectsEnabled)
				m_PostEffects->RenderPostEffects(m_Framebuffer, m_PostEffectsBuffer, m_ScreenQuad, m_IBO);

			// Display Framebuffer - potentially move to Framebuffer class
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer));
			Renderer::SetViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);
			Renderer::SetBlendFunction(RendererBlendFunction::SOURCE_ALPHA, RendererBlendFunction::ONE_MINUS_SOURCE_ALPHA));
			m_FramebufferMaterial->Bind();

			// TODO: None of this should be done here
			GLCall(glActiveTexture(GL_TEXTURE0));
			if (m_PostEffectsEnabled)
				m_PostEffectsBuffer->GetTexture()->Bind(m_FramebufferMaterial->GetShader());
			else
				m_Framebuffer->GetTexture()->Bind(m_FramebufferMaterial->GetShader());

			m_ScreenQuad->Bind();
			m_IBO->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
			m_IBO->Unbind();
			m_ScreenQuad->Unbind();
#endif
		}
	}

} }