#include "Model.h"
#include <tuple>
#include <unordered_map>

namespace std
{
	template<> struct hash<std::tuple<GLushort, GLushort, GLushort>>
	{
		const size_t operator()(const std::tuple<GLushort, GLushort, GLushort>& obj)
		{
			return std::get<0>(obj) ^ std::get<1>(obj) << 14 ^ std::get<2>(obj) << 23;
		}
	};
}

namespace sparky { namespace graphics { 

	Model::Model(const std::string& path, float scale)
		: m_Path(path), m_Scale(scale)
	{
		load();
	}

	void InsertVertex(const std::tuple<GLushort, GLushort, GLushort>& key,
		std::unordered_map<std::tuple<GLushort, GLushort, GLushort>, int>& mapping,
		const std::vector<maths::vec3>& positions,
		const std::vector<maths::vec3>& normals,
		const std::vector<maths::vec2>& uvs, 
		std::vector<GLushort>& indices, 
		std::vector<ModelVertexData>& vertexData)
	{
		auto lookup = mapping.find(key);
		if (lookup != mapping.end())
		{
			indices.push_back(lookup->second);
		}
		else
		{
			mapping[key] = (int)vertexData.size();
			indices.push_back(vertexData.size());
			ModelVertexData mvd =
			{ 
				positions[std::get<0>(key)],
				normals[std::get<2>(key)],
				uvs[std::get<1>(key)],
				0.0f,
				0xffffffff
			};
			vertexData.push_back(mvd);
		}
	}

	void Model::load()
	{
		using namespace maths;

		std::vector<vec3> vertices, normals;
		std::vector<vec2> uvs;

		FILE* file = fopen(m_Path.c_str(), "rt");
		int res;
		char header[128];
		std::unordered_map<std::tuple<GLushort, GLushort, GLushort>, int> mapping;

		std::vector<GLushort> indices;
		std::vector<ModelVertexData> vertexData;
		while ((res = fscanf(file, "%s", header)) != EOF)
		{
			if (strcmp(header, "v") == 0)
			{
				maths::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertex.x *= m_Scale;
				vertex.y *= m_Scale;
				vertex.z *= m_Scale;
				vertices.push_back(vertex);
			}
			else if (strcmp(header, "vt") == 0)
			{
				maths::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				uvs.push_back(uv);
			}
			else if (strcmp(header, "vn") == 0)
			{
				maths::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			else if (strcmp(header, "f") == 0)
			{
				GLushort index[3];
				GLushort tex[3];
				GLushort normal[3];
				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &index[0], &tex[0], &normal[0], &index[1], &tex[1], &normal[1], &index[2], &tex[2], &normal[2]);

				std::tuple<GLushort, GLushort, GLushort> i0 = std::make_tuple(index[0] - 1, tex[0] - 1, normal[0] - 1);
				std::tuple<GLushort, GLushort, GLushort> i1 = std::make_tuple(index[1] - 1, tex[1] - 1, normal[1] - 1);
				std::tuple<GLushort, GLushort, GLushort> i2 = std::make_tuple(index[2] - 1, tex[2] - 1, normal[2] - 1);

				InsertVertex(i0, mapping, vertices, normals, uvs, indices, vertexData);
				InsertVertex(i1, mapping, vertices, normals, uvs, indices, vertexData);
				InsertVertex(i2, mapping, vertices, normals, uvs, indices, vertexData);
			}
		}

		m_IBO = new IndexBuffer(&indices[0], indices.size());
		m_IndexCount = indices.size();

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, MODEL_SIZE(vertexData.size()), NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_NORMAL_INDEX);
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glEnableVertexAttribArray(SHADER_TID_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);

		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, MODEL_VERTEX_SIZE, (const GLvoid*)0);
		glVertexAttribPointer(SHADER_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, MODEL_VERTEX_SIZE, (const GLvoid*)(offsetof(ModelVertexData, normal)));
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, MODEL_VERTEX_SIZE, (const GLvoid*)(offsetof(ModelVertexData, uv)));
		glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, MODEL_VERTEX_SIZE, (const GLvoid*)(offsetof(ModelVertexData, tid)));
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, MODEL_VERTEX_SIZE, (const GLvoid*)(offsetof(ModelVertexData, color)));

		ModelVertexData* buffer = (ModelVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(buffer, &vertexData[0], MODEL_SIZE(vertexData.size()));
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Model::render()
	{
		glBindVertexArray(m_VAO);
		m_IBO->bind();
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, NULL);
	}

} }