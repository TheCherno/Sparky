#pragma once

#include "sp/graphics/API/Framebuffer2D.h"

namespace sp { namespace graphics {

	class D3DFramebuffer2D : public Framebuffer2D
	{
	private:
		uint m_Width, m_Height;
		maths::vec4 m_ClearColor;
	public:
		D3DFramebuffer2D(uint width, uint height);
		~D3DFramebuffer2D();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		inline uint GetWidth() const override { return m_Width; }
		inline uint GetHeight() const override { return m_Height; }

		inline API::Texture* GetTexture() const { return nullptr; }
		inline void SetClearColor(const maths::vec4& color) override { m_ClearColor = color; }
	private:
		void Init();
	};

} }
