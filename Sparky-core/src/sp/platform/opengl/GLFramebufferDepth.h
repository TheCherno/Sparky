#pragma once

#include "sp/graphics/API/FramebufferDepth.h"
#include "GLTextureDepth.h"
#include "GLCommon.h"

namespace sp { namespace graphics {

	class GLFramebufferDepth : public FramebufferDepth
	{
	private:
		uint m_FramebufferID;

		uint m_Width, m_Height;
		API::GLTextureDepth* m_Texture;
	public:
		GLFramebufferDepth(uint width, uint height);
		~GLFramebufferDepth();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		inline uint GetWidth() const override { return m_Width; }
		inline uint GetHeight() const override { return m_Height; }

		inline API::Texture* GetTexture() const override { return m_Texture; }
	private:
		void Init();
	};

} }
