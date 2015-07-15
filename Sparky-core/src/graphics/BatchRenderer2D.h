#pragma once

#include <cstddef>
#include <utils/Log.h>
#include <sparky_types.h>

#include "renderer2d.h"
#include "renderable2d.h"
#include "Framebuffer.h"

#include "buffers/indexbuffer.h"

namespace sparky { namespace graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32 - 1

	enum class RenderTarget
	{
		SCREEN = 0,
		BUFFER = 1
	};

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
		int m_ScreenBuffer;
		maths::tvec2<uint> m_ViewportSize, m_ScreenSize;
		Shader* m_SimpleShader;
		uint m_ScreenQuad;
		RenderTarget m_Target;
	public:
		BatchRenderer2D(const maths::tvec2<uint>& screenSize);
		~BatchRenderer2D();

		void begin() override;
		void submit(const Renderable2D* renderable) override;
		void drawString(const std::string& text, const maths::vec3& position, const Font& font, unsigned int color) override;
		void end() override;
		void flush() override;

		inline void SetScreenSize(const maths::tvec2<uint>& size) { m_ScreenSize = size; }
		inline const maths::tvec2<uint>& GetScreenSize() const { return m_ScreenSize; }
		inline void SetViewportSize(const maths::tvec2<uint>& size) { m_ViewportSize = size; }
		inline const maths::tvec2<uint>& GetViewportSize() const { return m_ViewportSize; }

		inline void SetRenderTarget(RenderTarget target) { m_Target = target; }
		inline const RenderTarget GetRenderTarget() const { return m_Target; }
	private:
		void init();
		float submitTexture(uint textureID);
		float submitTexture(const Texture* texture);
	};

} }