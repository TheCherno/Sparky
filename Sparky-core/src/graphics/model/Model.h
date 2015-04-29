#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>

#include "../../maths/maths.h"

namespace sparky { namespace graphics { 

	class Model
	{
	private:
		std::string m_Path;
		float m_Scale;
		GLuint m_VAO, m_VBO;
	public:
		Model(const std::string& path, float scale = 1.0f);
	private:
		void load();

	};

} }