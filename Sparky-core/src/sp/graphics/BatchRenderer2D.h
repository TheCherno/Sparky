#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "Renderer2D.h"
#include "Renderable2D.h"
#include "Framebuffer.h"
#include "FontManager.h"

#include "buffers/VertexArray.h"
#include "buffers/IndexBuffer.h"

namespace sp { namespace graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32 - 1

	class SP_API BatchRenderer2D : public Renderer2D
	{
	private:
		VertexArray* m_VertexArray;
		uint m_VAO;
		uint m_VBO;
		IndexBuffer* m_IBO;
		IndexBuffer* m_LineIBO;
		uint m_IndexCount, m_LineIndexCount;
		VertexData* m_Buffer;
#ifdef SPARKY_PLATFORM_WEB
		VertexData* m_BufferBase;
#endif
		std::vector<uint> m_TextureSlots;
		Framebuffer* m_Framebuffer;
		Framebuffer* m_PostEffectsBuffer;
		int m_ScreenBuffer;
		maths::tvec2<uint> m_ViewportSize, m_ScreenSize;
		Shader* m_SimpleShader;
		VertexArray* m_ScreenQuad;
	public:
		BatchRenderer2D(uint width, uint height);
		BatchRenderer2D(const maths::tvec2<uint>& screenSize);
		~BatchRenderer2D();

		void Begin() override;
		void Submit(const Renderable2D* renderable) override;

		void DrawLine(float x0, float y0, float x1, float y1, float thickness = 0.02f, uint color = 0xffffffff) override;
		void DrawLine(const maths::vec2& start, const maths::vec2& end, float thickness = 0.02f, uint color = 0xffffffff) override;
		void DrawRect(float x, float y, float width, float height, uint color = 0xffffffff) override;
		void DrawRect(const maths::Rectangle& rectangle, uint color = 0xffffffff) override;
		void DrawString(const String& text, const maths::vec2& position, const Font& font = *FontManager::Get(), uint color = 0xffffffff) override;

		void FillRect(float x, float y, float width, float height, uint color = 0xffffffff) override;
		void FillRect(const maths::Rectangle& rectangle, uint color = 0xffffffff) override;

		void End() override;
		void Present() override;

		inline void SetScreenSize(const maths::tvec2<uint>& size) { m_ScreenSize = size; }
		inline const maths::tvec2<uint>& GetScreenSize() const { return m_ScreenSize; }
		inline void SetViewportSize(const maths::tvec2<uint>& size) { m_ViewportSize = size; }
		inline const maths::tvec2<uint>& GetViewportSize() const { return m_ViewportSize; }
	private:
		void Init();

		// TODO: Deprecate. Only sp::graphics::Texture should be allowable.
		float SubmitTexture(uint textureID);

		float SubmitTexture(const Texture* texture);
	};

} }