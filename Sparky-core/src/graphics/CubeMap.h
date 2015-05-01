#pragma once

#include <iostream>
#include <GL/glew.h>
#include "../utils/ImageLoad.h"

namespace sparky { namespace graphics {

	class CubeMap
	{
	private:
		GLuint m_TID;
		std::string m_Path;
		unsigned int m_Width, m_Height;
	public:
		CubeMap(const std::string& path, unsigned int width, unsigned int height);
		void bind() const;
		void unbind() const;
	private:
		void load();
	};

} }