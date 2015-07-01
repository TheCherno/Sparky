#pragma once

#include <iostream>
#include <string>
#include <utils/Log.h>
#include <FreeImage.h>
#include <sparkygl.h>

#include "../utils/ImageLoad.h"

namespace sparky { namespace graphics{

	enum TextureWrap
	{
		REPEAT			= GL_REPEAT,
		CLAMP			= GL_CLAMP,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE	= GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER	= GL_CLAMP_TO_BORDER
	};

	class Texture
	{
	private:
		static TextureWrap s_WrapMode;
	private:
		std::string m_Name, m_FileName;
		GLuint m_TID;
		GLsizei m_Width, m_Height;
		unsigned int m_Bits;
	public:
		Texture(const std::string& name, const std::string& filename);
		~Texture();
		void bind() const;
		void unbind() const;
		
		inline const std::string& getName() const { return m_Name; }
		inline const unsigned int getID() const { return m_TID; }
		inline const unsigned int getWidth() const { return m_Width; }
		inline const unsigned int getHeight() const { return m_Height; }
	public:
		inline static void SetWrap(TextureWrap mode) { s_WrapMode = mode; }
	private:
		GLuint load();
	};

} }