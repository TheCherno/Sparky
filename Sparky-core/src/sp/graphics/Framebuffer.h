#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "sp/maths/vec4.h"
#include "sp/maths/tvec2.h"

#include "Texture.h"

namespace sp { namespace graphics {

	class SP_API Framebuffer
	{
	private:
		struct FramebufferData
		{
			uint framebufferID;
			uint depthbufferID;
		};

		Texture* m_Texture;
		FramebufferData m_Data;
		maths::tvec2<uint> m_Size;
		uint& m_Width;
		uint& m_Height;

		maths::vec4 m_ClearColor;
	public:
		Framebuffer(const maths::tvec2<uint>& size);
		Framebuffer(uint width, uint height);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;
		void Clear();

		inline const maths::tvec2<uint>& GetSize() const { return m_Size; }
		inline const uint GetWidth() const { return m_Width; }
		inline const uint GetHeight() const { return m_Height; }
		inline const Texture* GetTexture() const { return m_Texture; }

		inline void SetClearColor(const maths::vec4& color) { m_ClearColor = color; }
	private:
		void Create(uint width, uint height);
	};

} }