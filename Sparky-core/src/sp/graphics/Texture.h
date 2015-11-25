#pragma once

#include <iostream>
#include <string>

#include <FreeImage.h>

#include <sp/Types.h>
#include "SPRenderAPI.h"

#include <sp/utils/ImageLoad.h>

namespace sp { namespace graphics{

	enum class TextureWrap
	{
		REPEAT			= GL_REPEAT,
		CLAMP			= GL_CLAMP,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE	= GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER	= GL_CLAMP_TO_BORDER
	};

	enum class TextureFilter
	{
		LINEAR = GL_LINEAR,
		NEAREST = GL_NEAREST
	};

	class Texture
	{
	private:
		static TextureWrap s_WrapMode;
		static TextureFilter s_FilterMode;
	private:
		String m_Name, m_FileName;
		uint m_TID;
		uint m_Width, m_Height;
		unsigned int m_Bits;
	public:
		Texture(uint width, uint height, uint bits = 24);
		Texture(const String& name, const String& filename);
		~Texture();
		void Bind() const;
		void Unbind() const;
		
		inline const String& GetName() const { return m_Name; }
		inline const unsigned int GetID() const { return m_TID; }
		inline const unsigned int GetWidth() const { return m_Width; }
		inline const unsigned int GetHeight() const { return m_Height; }
	public:
		inline static void SetWrap(TextureWrap mode) { s_WrapMode = mode; }
		inline static void SetFilter(TextureFilter mode) { s_FilterMode = mode; }
	private:
		GLuint Load();
	};

} }