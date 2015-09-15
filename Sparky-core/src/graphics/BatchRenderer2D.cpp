#include "BatchRenderer2D.h"

#include "shaders/ShaderFactory.h"
#include "MeshFactory.h"

#include <utils/Log.h>

namespace sparky { namespace graphics {

	using namespace maths;

	BatchRenderer2D::BatchRenderer2D(const maths::tvec2<uint>& screenSize)
		: m_IndexCount(0), m_ScreenSize(screenSize), m_ViewportSize(screenSize)
	{
		Init();
	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		delete m_IBO;
		GLCall(glDeleteBuffers(1, &m_VBO));
		GLCall(glDeleteVertexArrays(1, &m_VAO));
	}

	void BatchRenderer2D::Init()
	{
		GLCall(glGenVertexArrays(1, &m_VAO));
		GLCall(glGenBuffers(1, &m_VBO));

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW));

		GLCall(glEnableVertexAttribArray(SHADER_VERTEX_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_UV_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_MASK_UV_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_TID_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_MID_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_COLOR_INDEX));

		GLCall(glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0));
		GLCall(glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, uv))));
		GLCall(glVertexAttribPointer(SHADER_MASK_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, mask_uv))));
		GLCall(glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, tid))));
		GLCall(glVertexAttribPointer(SHADER_MID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, mid))));
		GLCall(glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, color))));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[  i  ] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

		GLCall(glBindVertexArray(0));

#ifdef SPARKY_PLATFORM_WEB
		m_BufferBase = new VertexData[RENDERER_MAX_SPRITES * 4];
#endif

		// Setup Framebuffer
		GLCall(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_ScreenBuffer));
		m_Framebuffer = new Framebuffer(m_ViewportSize);
		m_SimpleShader = ShaderFactory::SimpleShader();
		m_SimpleShader->Bind();
		m_SimpleShader->SetUniformMat4("pr_matrix", maths::mat4::Orthographic(0, m_ScreenSize.x, m_ScreenSize.y, 0, -1.0f, 1.0f));
		m_SimpleShader->SetUniform1i("tex", 0);
		m_SimpleShader->Unbind();
		m_ScreenQuad = MeshFactory::CreateQuad(0, 0, m_ScreenSize.x, m_ScreenSize.y);

		m_PostEffects = new PostEffects();
		m_PostEffectsBuffer = new Framebuffer(m_ViewportSize);
	}

	float BatchRenderer2D::SubmitTexture(uint textureID)
	{
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
				Flush();
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
		}
		else
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer));
			GLCall(glViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y));
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
#ifdef SPARKY_PLATFORM_WEB
		m_Buffer = m_BufferBase;
#else
		GLCall(m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
#endif
	}

	void BatchRenderer2D::Submit(const Renderable2D* renderable)
	{
		const maths::vec3& position = renderable->GetPosition();
		const maths::vec2& size = renderable->GetSize();
		const unsigned int color = renderable->GetColor();
		const std::vector<maths::vec2>& uv = renderable->GetUV();
		const GLuint tid = renderable->GetTID();

		float ts = 0.0f;
		if (tid > 0)
			ts = SubmitTexture(renderable->GetTexture());

		maths::mat4 maskTransform = maths::mat4::Identity();
		const GLuint mid = m_Mask ? m_Mask->texture->GetID() : 0;
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

	void BatchRenderer2D::DrawString(const std::string& text, const maths::vec3& position, const Font& font, unsigned int color)
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
			if (glyph != NULL)
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

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(x0, y0, 0);
				m_Buffer->uv = maths::vec2(u0, v0);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(x0, y1, 0);
				m_Buffer->uv = maths::vec2(u0, v1);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(x1, y1, 0);
				m_Buffer->uv = maths::vec2(u1, v1);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(x1, y0, 0);
				m_Buffer->uv = maths::vec2(u1, v0);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_IndexCount += 6;

				x += glyph->advance_x / scale.x;
			}
		}
	}

	void BatchRenderer2D::End()
	{
#ifdef SPARKY_PLATFORM_WEB
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, (m_Buffer - m_BufferBase) * RENDERER_VERTEX_SIZE, m_BufferBase);
		m_Buffer = m_BufferBase;
#else
		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
#endif
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void BatchRenderer2D::Flush()
	{
		for (uint i = 0; i < m_TextureSlots.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]));
		}

		GLCall(glBindVertexArray(m_VAO));
		m_IBO->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL));

		m_IBO->Unbind();
		GLCall(glBindVertexArray(0));

		m_IndexCount = 0;
		m_TextureSlots.clear();
		
		if (m_Target == RenderTarget::BUFFER)
		{
			// Post Effects pass should go here!
			if (m_PostEffectsEnabled)
				m_PostEffects->RenderPostEffects(m_Framebuffer, m_PostEffectsBuffer, m_ScreenQuad, m_IBO);

			// Display Framebuffer - potentially move to Framebuffer class
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBuffer));
			GLCall(glViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y));
			m_SimpleShader->Bind();

			GLCall(glActiveTexture(GL_TEXTURE0));
			if (m_PostEffectsEnabled)
				m_PostEffectsBuffer->GetTexture()->Bind();
			else
				m_Framebuffer->GetTexture()->Bind();

			GLCall(glBindVertexArray(m_ScreenQuad));
			m_IBO->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
			m_IBO->Unbind();
			GLCall(glBindVertexArray(0));
			m_SimpleShader->Unbind();
		}
	}

} }