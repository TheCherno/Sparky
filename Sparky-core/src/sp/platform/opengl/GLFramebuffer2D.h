#pragma once

#include "sp/graphics/API/Framebuffer2D.h"
#include "GLTexture2D.h"
#include "GLCommon.h"

namespace sp { namespace graphics {

	class GLFramebuffer2D : public Framebuffer2D
	{
	private:
		uint m_FramebufferHandle;
		uint m_DepthbufferHandle;

		uint m_Width, m_Height;
		maths::vec4 m_ClearColor;
		API::GLTexture2D* m_Texture;
	public:
		GLFramebuffer2D(uint width, uint height);
		~GLFramebuffer2D();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		inline uint GetWidth() const override { return m_Width; }
		inline uint GetHeight() const override { return m_Height; }

		inline API::Texture* GetTexture() const override { return m_Texture; }
		inline API::Texture2D* GetTexture2D() const override { return m_Texture; }
		inline void SetClearColor(const maths::vec4& color) override { m_ClearColor = color; }
	private:
		void Init();
	};

} }
