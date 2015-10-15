#pragma once

#include <cstddef>

#include <sparky_types.h>

#include "Renderer2D.h"
#include "Renderable2D.h"
#include "Framebuffer.h"

#include "buffers/IndexBuffer.h"

namespace sparky { namespace graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32 - 1

	class BatchRenderer2D : public Renderer2D
	{
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		IndexBuffer* m_IBO;
		GLsizei m_IndexCount;
		VertexData* m_Buffer;
#ifdef SPARKY_PLATFORM_WEB
		VertexData* m_BufferBase;
#endif
		std::vector<GLuint> m_TextureSlots;
		Framebuffer* m_Framebuffer;
		Framebuffer* m_PostEffectsBuffer;
		int m_ScreenBuffer;
		maths::tvec2<uint> m_ViewportSize, m_ScreenSize;
		Shader* m_SimpleShader;
		uint m_ScreenQuad;
	public:
		BatchRenderer2D(uint width, uint height);
		BatchRenderer2D(const maths::tvec2<uint>& screenSize);
		~BatchRenderer2D();

		void Begin() override;
		void Submit(const Renderable2D* renderable) override;
		void DrawString(const std::string& text, const maths::vec3& position, const Font& font, unsigned int color) override;
		void End() override;
		void Flush() override;

		inline void SetScreenSize(const maths::tvec2<uint>& size) { m_ScreenSize = size; }
		inline const maths::tvec2<uint>& GetScreenSize() const { return m_ScreenSize; }
		inline void SetViewportSize(const maths::tvec2<uint>& size) { m_ViewportSize = size; }
		inline const maths::tvec2<uint>& GetViewportSize() const { return m_ViewportSize; }
	private:
		void Init();
		float SubmitTexture(uint textureID);
		float SubmitTexture(const Texture* texture);
	};

} }