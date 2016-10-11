#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "Renderable2D.h"
#include "API/Framebuffer2D.h"
#include "FontManager.h"
#include "Mask.h"
#include "camera/Camera.h"
#include "postfx/PostEffects.h"

#include "API/VertexArray.h"
#include "API/IndexBuffer.h"

namespace sp { namespace graphics {

	enum class SP_API RenderTarget
	{
		SCREEN = 0,
		BUFFER = 1
	};

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

	class SP_API Renderer2D
	{
	private:
		static bool s_PostEffectsEnabled;
		static bool s_MaskEnabled;
	private:
		std::vector<maths::mat4> m_TransformationStack;
		const maths::mat4* m_TransformationBack;
		const Mask* m_Mask;
		RenderTarget m_Target;
		PostEffects* m_PostEffects;
		bool m_PostEffectsEnabled;

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
		Renderer2D(uint width, uint height);
		Renderer2D(const maths::tvec2<uint>& screenSize);
		~Renderer2D();

		void Push(const maths::mat4& matrix, bool override = false);
		void Pop();

		void SetCamera(Camera* camera);

		// Staging Functions
		void Begin();
		void Submit(const Renderable2D* renderable);
		void End();
		void Present();

		// Drawing Functions
		void DrawLine(float x0, float y0, float x1, float y1, uint color = 0xffffffff, float thickness = 0.02f);
		void DrawLine(const maths::vec2& start, const maths::vec2& end, uint color = 0xffffffff, float thickness = 0.02f);
		void DrawRect(float x, float y, float width, float height, uint color = 0xffffffff);
		void DrawRect(const maths::vec2& position, const maths::vec2& size, uint color = 0xffffffff);
		void DrawRect(const maths::Rectangle& rectangle, uint color = 0xffffffff);
		void DrawString(const String& text, const maths::vec2& position, const Font& font = *FontManager::Get(), uint color = 0xffffffff);

		void FillRect(float x, float y, float width, float height, uint color = 0xffffffff);
		void FillRect(const maths::vec2& position, const maths::vec2& size, uint color = 0xffffffff);
		void FillRect(const maths::Rectangle& rectangle, uint color = 0xffffffff);

		inline void SetRenderTarget(RenderTarget target) { m_Target = target; }
		inline const RenderTarget GetRenderTarget() const { return m_Target; }

		inline void SetMask(const Mask* mask) { m_Mask = mask; }
		inline void SetPostEffects(bool enabled) { m_PostEffectsEnabled = enabled; }
		inline bool GetPostEffects() const { return m_PostEffectsEnabled; }
		inline void AddPostEffectsPass(PostEffectsPass* pass) { m_PostEffects->Push(pass); }

		inline void SetScreenSize(const maths::tvec2<uint>& size) { m_ScreenSize = size; }
		inline const maths::tvec2<uint>& GetScreenSize() const { return m_ScreenSize; }
		inline void SetViewportSize(const maths::tvec2<uint>& size) { m_ViewportSize = size; }
		inline const maths::tvec2<uint>& GetViewportSize() const { return m_ViewportSize; }
	private:
		void Init();
		float SubmitTexture(API::Texture* texture);
	};

} }