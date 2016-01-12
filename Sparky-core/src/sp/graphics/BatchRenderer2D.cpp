#include "sp/sp.h"
#include "BatchRenderer2D.h"

#include "shaders/ShaderFactory.h"
#include "MeshFactory.h"

#include "buffers/Buffer.h"
#include "buffers/BufferLayout.h"

#include "sp/utils/Log.h"
#include "SPRenderAPI.h"

#include <freetype-gl/freetype-gl.h>

namespace sp { namespace graphics {

	using namespace maths;

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
		delete m_ScreenQuad;
		delete m_IBO;
		API::FreeBuffer(m_VBO);
		API::FreeVertexArray(m_VAO);
	}

	void BatchRenderer2D::Init()
	{
		API::Buffer* buffer = new API::Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		buffer->Bind();
		buffer->Resize(RENDERER_BUFFER_SIZE);

		buffer->layout.Push<vec3>("position");
		buffer->layout.Push<vec2>("uv");
		buffer->layout.Push<vec2>("mask_uv");
		buffer->layout.Push<float>("tid");
		buffer->layout.Push<float>("mid");
		buffer->layout.Push<byte>("color", 4, true);

		m_VertexArray = new VertexArray();
		m_VertexArray->Bind();
		m_VertexArray->PushBuffer(buffer);

		uint* indices = new uint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);
		m_VertexArray->Unbind();

#ifdef SPARKY_PLATFORM_WEB
		m_BufferBase = new VertexData[RENDERER_MAX_SPRITES * 4];
#endif

		// Setup Framebuffer
		m_ScreenBuffer = API::GetScreenBuffer();
		SP_ASSERT(m_ScreenBuffer == 0);

		m_Framebuffer = new Framebuffer(m_ViewportSize);
		m_SimpleShader = ShaderFactory::SimpleShader();
		m_SimpleShader->Bind();
		m_SimpleShader->SetUniformMat4("pr_matrix", maths::mat4::Orthographic(0, (float)m_ScreenSize.x, (float)m_ScreenSize.y, 0, -1.0f, 1.0f));
		m_SimpleShader->SetUniform1i("tex", 0);
		m_SimpleShader->Unbind();
		m_ScreenQuad = MeshFactory::CreateQuad(0, 0, (float)m_ScreenSize.x, (float)m_ScreenSize.y);

		m_PostEffects = new PostEffects();
		m_PostEffectsBuffer = new Framebuffer(m_ViewportSize);
	}

	float BatchRenderer2D::SubmitTexture(uint textureID)
	{
		if (!textureID)
			SP_WARN("Invalid texture ID submitted!");
		float result = 0.0f;
		bool found = false;
		for (uint i = 0; i < m_TextureSlots.size(); i++)
		{
			if (m_TextureSlots[i] == textureID)
			{
				result = (float)(i + 1);
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
			{
				End();
				Present();
				Begin();
			}
			m_TextureSlots.push_back(textureID);
			result = (float)(m_TextureSlots.size());
		}
		return result;
	}

	float BatchRenderer2D::SubmitTexture(const Texture* texture)
	{
		return SubmitTexture(texture->GetID());
	}

	void BatchRenderer2D::Begin()
	{
		if (m_Target == RenderTarget::BUFFER)
		{
			if (m_ViewportSize != m_Framebuffer->GetSize())
			{
				delete m_Framebuffer;
				m_Framebuffer = new Framebuffer(m_ViewportSize);
				
				if (m_PostEffectsEnabled)
				{
					delete m_PostEffectsBuffer;
					m_PostEffectsBuffer = new Framebuffer(m_ViewportSize);
				}
			}

			if (m_PostEffectsEnabled)
			{
				m_PostEffectsBuffer->Bind();
				m_PostEffectsBuffer->Clear();
			}

			m_Framebuffer->Bind();
			m_Framebuffer->Clear(); // TODO: Clear somewhere else, since this basically limits to one draw call
			glBlendFunc(GL_ONE, GL_ZERO);
		}
		else
		{
			API::BindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer);
			API::SetViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);
		}
		m_VertexArray->GetBuffer()->Bind();
#ifdef SPARKY_PLATFORM_WEB
		m_Buffer = m_BufferBase;
#else
		m_Buffer = m_VertexArray->GetBuffer()->GetPointer<VertexData>();
#endif
	}

	void BatchRenderer2D::Submit(const Renderable2D* renderable)
	{
		if (!renderable->IsVisible())
			return;

		const vec3& position = renderable->GetPosition();
		const vec2& size = renderable->GetSize();
		const uint color = renderable->GetColor();
		const std::vector<vec2>& uv = renderable->GetUV();
		const GLuint tid = renderable->GetTID();

		float ts = 0.0f;
		if (tid > 0)
			ts = SubmitTexture(renderable->GetTexture());

		mat4 maskTransform = mat4::Identity();
		const uint mid = m_Mask ? m_Mask->texture->GetID() : 0;
		float ms = 0.0f;

		if (m_Mask != nullptr)
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

		vertex = *m_TransformationBack * vec3(position.x, position.y + size.y, position.z);
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

		vertex = *m_TransformationBack * vec3(position.x + size.x, position.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::DrawLine(float x0, float y0, float x1, float y1, float thickness, uint color)
	{
		const std::vector<vec2>& uv = Renderable2D::GetDefaultUVs();
		float ts = 0.0f;
		mat4 maskTransform = mat4::Identity();
		uint mid = m_Mask ? m_Mask->texture->GetID() : 0;

		float ms = 0.0f;
		if (m_Mask != nullptr)
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

	void BatchRenderer2D::DrawLine(const maths::vec2& start, const maths::vec2& end, float thickness, uint color)
	{
		DrawLine(start.x, start.y, end.x, end.y, thickness, color);
	}

	void BatchRenderer2D::DrawRect(float x, float y, float width, float height, uint color)
	{
		DrawLine(x, y, x + width, y);
		DrawLine(x + width, y, x + width, y + height);
		DrawLine(x + width, y + height, x, y + height);
		DrawLine(x, y + height, x, y);
	}

	void BatchRenderer2D::DrawRect(const Rectangle& rectangle, uint color)
	{
		DrawRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height, color);
	}

	void BatchRenderer2D::DrawString(const String& text, const maths::vec2& position, const Font& font, uint color)
	{
		using namespace ftgl;

		float ts = 0.0f;
		ts = SubmitTexture(font.GetID());

		const maths::vec2& scale = font.GetScale();

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
		uint mid = m_Mask ? m_Mask->texture->GetID() : 0;

		float ms = 0.0f;
		if (m_Mask != nullptr)
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

		vertex = *m_TransformationBack * vec3(position.x, position.y + size.y, position.z);
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

		vertex = *m_TransformationBack * vec3(position.x + size.x, position.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::FillRect(const Rectangle& rectangle, uint color)
	{
		FillRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height, color);
	}

	void BatchRenderer2D::End()
	{
#ifdef SPARKY_PLATFORM_WEB
		API::BindBuffer(GL_ARRAY_BUFFER, m_VBO);
		API::SetBufferSubData(GL_ARRAY_BUFFER, 0, (m_Buffer - m_BufferBase) * RENDERER_VERTEX_SIZE, m_BufferBase);
		m_Buffer = m_BufferBase;
#else
		m_VertexArray->GetBuffer()->ReleasePointer();
#endif
		m_VertexArray->GetBuffer()->Unbind();
	}

	void BatchRenderer2D::Present()
	{
		GLCall(glDepthFunc(GL_NEVER));
		GLCall(glDisable(GL_DEPTH_TEST));

		for (uint i = 0; i < m_TextureSlots.size(); i++)
		{
			API::SetActiveTexture(GL_TEXTURE0 + i);
			API::BindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
		}

		// Draw buffers here
		{
			m_VertexArray->Bind();
			m_IBO->Bind();

			API::DrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

			m_IBO->Unbind();
			m_VertexArray->Unbind();
		}

		m_IndexCount = 0;
		m_TextureSlots.clear();
		
		if (m_Target == RenderTarget::BUFFER)
		{
			// Post Effects pass should go here!
			if (m_PostEffectsEnabled)
				m_PostEffects->RenderPostEffects(m_Framebuffer, m_PostEffectsBuffer, m_ScreenQuad, m_IBO);

			// Display Framebuffer - potentially move to Framebuffer class
			API::BindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer);
			API::SetViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			m_SimpleShader->Bind();

			API::SetActiveTexture(GL_TEXTURE0);
			if (m_PostEffectsEnabled)
				m_PostEffectsBuffer->GetTexture()->Bind();
			else
				m_Framebuffer->GetTexture()->Bind();

			m_ScreenQuad->Bind();
			m_IBO->Bind();
			API::DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			m_IBO->Unbind();
			m_ScreenQuad->Unbind();

			m_SimpleShader->Unbind();
		}
	}

} }