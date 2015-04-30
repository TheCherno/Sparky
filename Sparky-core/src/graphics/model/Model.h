#pragma once

#include <iostream>
#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <GL/glew.h>

#include "../buffers/indexbuffer.h"
#include "../../maths/maths.h"

namespace sparky { namespace graphics { 

#define SHADER_VERTEX_INDEX 0
#define SHADER_NORMAL_INDEX	1
#define SHADER_UV_INDEX		2
#define SHADER_TID_INDEX	3
#define SHADER_COLOR_INDEX	4

	struct ModelVertexData
	{
		maths::vec3 vertex;
		maths::vec3 normal;
		maths::vec2 uv;
		float tid;
		unsigned int color;
	};

#define MODEL_VERTEX_SIZE sizeof(ModelVertexData)
#define MODEL_SIZE(x) x * sizeof(ModelVertexData)

	class Model
	{
	private:
		std::string m_Path;
		float m_Scale;
		GLuint m_VAO, m_VBO;
		IndexBuffer* m_IBO;
		GLsizei m_IndexCount;
	public:
		Model(const std::string& path, float scale = 1.0f);
		void render();
	private:
		void load();

	};

} }