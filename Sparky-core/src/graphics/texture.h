#pragma once

#include <iostream>
#include <string>
#include <utils/Log.h>
#include <FreeImage.h>

#ifdef SPARKY_EMSCRIPTEN
	#define GLFW_INCLUDE_ES3
	#include <GLFW/glfw3.h>
#else
	#include <GL/glew.h>
#endif

#include "../utils/ImageLoad.h"

namespace sparky { namespace graphics{

	class Texture
	{
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
	private:
		GLuint load();
	};

} }