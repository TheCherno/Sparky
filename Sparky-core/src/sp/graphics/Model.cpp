#include "sp/sp.h"
#include "Model.h"

#include <sp/String.h>
#include <sp/utils/StringUtils.h>
#include <sp/utils/FileUtils.h>

#include <GL/glew.h>

template<>
struct std::hash<sp::graphics::Model::IndexSet>
{
	const size_t operator()(const sp::graphics::Model::IndexSet& key) const
	{
		return (key.position) ^ (key.normal << 14) ^ (key.uv << 23);
	}
};

namespace sp { namespace graphics {

	using namespace maths;

	Model::Model(const String& path, MaterialInstance* materialInstance)
	{
		Load(path);
		m_Mesh->SetMaterial(materialInstance);
	}

	Model::~Model()
	{
		delete m_Mesh;
	}

	void Model::Load(const String& path)
	{
		std::vector<String> lines = utils::SplitString(utils::ReadFile(path), '\n');
		VertexSet inputVertices;
		std::vector<Vertex> vertices;
		std::vector<uint> indices;
		std::unordered_map<IndexSet, int> mapping;

		for (String line : lines)
		{
			const char* cstr = line.c_str();
			if (strstr(cstr, "#")) // Comment
			{
				continue;
			}
			else if (strstr(cstr, "v"))
			{
				if (strstr(cstr, "vt"))
				{
					vec2 uv;
					int result = sscanf(cstr, "%*s %f %f", &uv.x, &uv.y);
					if (result == 0)
						continue;
					inputVertices.uvs.push_back(uv);
				}
				else if (strstr(cstr, "vn"))
				{
					vec3 normal;
					int result = sscanf(cstr, "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
					if (result == 0)
						continue;
					inputVertices.normals.push_back(normal);
				}
				else
				{
					vec3 position;
					int result = sscanf(cstr, "%*s %f %f %f", &position.x, &position.y, &position.z);
					if (result == 0)
						continue;
					inputVertices.positions.push_back(position);
				}
			}
			else if (strstr(cstr, "f"))
			{
				IndexSet indexSet[3];
				int result = sscanf(cstr, "%*s %d/%d/%d %d/%d/%d %d/%d/%d", &indexSet[0].position, &indexSet[0].uv, &indexSet[0].normal, &indexSet[1].position, &indexSet[1].uv, &indexSet[1].normal, &indexSet[2].position, &indexSet[2].uv, &indexSet[2].normal);
				if (result == 0)
					continue;

				InsertVertex(vertices, indices, mapping, inputVertices, indexSet[0]);
				InsertVertex(vertices, indices, mapping, inputVertices, indexSet[1]);
				InsertVertex(vertices, indices, mapping, inputVertices, indexSet[2]);
			}
		}

		API::Buffer* buffer = new API::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer->Bind();
		buffer->SetData(vertices.size() * sizeof(Vertex), &vertices[0]);

		buffer->layout.Push<vec3>("position");
		buffer->layout.Push<vec3>("normal");
		buffer->layout.Push<vec2>("uv");

		VertexArray* va = new VertexArray();
		va->Bind();
		va->PushBuffer(buffer);

		IndexBuffer* ib = new IndexBuffer(&indices[0], indices.size());
		m_Mesh = new Mesh(va, ib, nullptr);
	}

	void Model::InsertVertex(std::vector<Vertex>& vertices, std::vector<uint>& indices, std::unordered_map<IndexSet, int>& mapping, VertexSet& inputVertices, IndexSet& indexSet)
	{
		auto lookup = mapping.find(indexSet);
		if (lookup != mapping.end())
		{
			indices.push_back(lookup->second);
		}
		else
		{
			mapping[indexSet] = (int)vertices.size();
			indices.push_back(vertices.size());
			Vertex vertex = { inputVertices.positions[indexSet.position - 1], inputVertices.normals[indexSet.normal - 1], inputVertices.uvs[indexSet.uv - 1] };
			vertices.push_back(vertex);
		}
	}

	void Model::Render(Renderer3D& renderer)
	{
		m_Mesh->Render(renderer);
	}

} }