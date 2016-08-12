#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "Renderer2D.h"
#include "Renderable2D.h"
#include "API/Framebuffer2D.h"
#include "FontManager.h"

#include "API/VertexArray.h"
#include "API/IndexBuffer.h"

namespace sp { namespace graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32 - 1

	struct UniformBuffer
	{
		byte* buffer;
		uint size;

		UniformBuffer() {}
		UniformBuffer(byte* buffer, uint size)
			: buffer(buffer), size(size)
		{
			memset(buffer, 0, size);
		}
	};

	struct BR2DSystemUniform
	{
		UniformBuffer buffer;
		uint offset;

		BR2DSystemUniform() {}
		BR2DSystemUniform(const UniformBuffer& buffer, uint offset)
			: buffer(buffer), offset(offset)
		{
		}
	};

	class SP_API BatchRenderer2D : public Renderer2D
	{
	private:
		API::Shader* m_Shader;
		std::vector<BR2DSystemUniform> m_SystemUniforms;
		std::vector<UniformBuffer> m_SystemUniformBuffers;

		API::VertexArray* m_VertexArray;
		API::IndexBuffer* m_IndexBuffer;
		API::IndexBuffer* m_LineIBO;
		uint m_IndexCount, m_LineIndexCount;
		VertexData* m_Buffer;
		std::vector<API::Texture*> m_Textures;
		Framebuffer2D* m_Framebuffer;
		Framebuffer2D* m_PostEffectsBuffer;
		maths::tvec2<uint> m_ViewportSize, m_ScreenSize;
		Material* m_FramebufferMaterial;
		API::VertexArray* m_ScreenQuad;
		Camera* m_Camera;
	public:
		BatchRenderer2D(uint width, uint height);
		BatchRenderer2D(const maths::tvec2<uint>& screenSize);
		~BatchRenderer2D();

		void SetCamera(Camera* camera) override;

		void Begin() override;
		void Submit(const Renderable2D* renderable) override;

		void DrawLine(float x0, float y0, float x1, float y1, uint color = 0xffffffff, float thickness = 0.02f) override;
		void DrawLine(const maths::vec2& start, const maths::vec2& end, uint color = 0xffffffff, float thickness = 0.02f) override;
		void DrawRect(float x, float y, float width, float height, uint color = 0xffffffff) override;
		void DrawRect(const maths::vec2& position, const maths::vec2& size, uint color = 0xffffffff) override;
		void DrawRect(const maths::Rectangle& rectangle, uint color = 0xffffffff) override;
		void DrawString(const String& text, const maths::vec2& position, const Font& font = *FontManager::Get(), uint color = 0xffffffff) override;

		void FillRect(float x, float y, float width, float height, uint color = 0xffffffff) override;
		void FillRect(const maths::vec2& position, const maths::vec2& size, uint color = 0xffffffff) override;
		void FillRect(const maths::Rectangle& rectangle, uint color = 0xffffffff) override;

		void End() override;
		void Present() override;

		inline void SetScreenSize(const maths::tvec2<uint>& size) { m_ScreenSize = size; }
		inline const maths::tvec2<uint>& GetScreenSize() const { return m_ScreenSize; }
		inline void SetViewportSize(const maths::tvec2<uint>& size) { m_ViewportSize = size; }
		inline const maths::tvec2<uint>& GetViewportSize() const { return m_ViewportSize; }
	private:
		void Init();
		float SubmitTexture(API::Texture* texture);
	};

} }