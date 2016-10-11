#include "sp/sp.h"
#include "MeshFactory.h"

#include "sp/graphics/Renderer2D.h"

#include "sp/graphics/Renderable2D.h"
#include "sp/graphics/shaders/Shader.h"

#include <GL/glew.h>

namespace sp { namespace graphics { namespace MeshFactory {

	Mesh* CreateQuad(float x, float y, float width, float height, MaterialInstance* material)
	{
		using namespace maths;

		struct QuadVertex
		{
			maths::vec3 position;
			maths::vec2 uv;
		};

		QuadVertex data[4];

		data[0].position = maths::vec3(x, y, 0);
		data[0].uv = maths::vec2(0, 1);

		data[1].position = maths::vec3(x + width, y, 0);
		data[1].uv = maths::vec2(0, 0);

		data[2].position = maths::vec3(x + width, y + height, 0);
		data[2].uv = maths::vec2(1, 0);

		data[3].position = maths::vec3(x, y + height, 0);
		data[3].uv = maths::vec2(1, 1);

		API::VertexBuffer* buffer = API::VertexBuffer::Create(API::BufferUsage::STATIC);
		buffer->SetData(sizeof(QuadVertex) * 4, data);

		API::BufferLayout layout;
		layout.Push<vec3>("POSITION");
		layout.Push<vec2>("TEXCOORD");
		buffer->SetLayout(layout);

		API::VertexArray* va = API::VertexArray::Create();
		va->PushBuffer(buffer);
		uint* indices = new uint[6] { 0, 1, 2, 2, 3, 0, };
		API::IndexBuffer* ib = API::IndexBuffer::Create(indices, 6);
		return new Mesh(va, ib, material);
	}

	Mesh* CreateQuad(const maths::vec2& position, const maths::vec2& size, MaterialInstance* material)
	{
		return CreateQuad(position.x, position.y, size.x, size.y, material);
	}

	Mesh* CreateCube(float size, MaterialInstance* material)
	{
		// TODO: Do this ~properly~! Current lazy cube.

		using namespace maths;

		Vertex data[8];

		memset(data, 0, 8 * sizeof(Vertex));

		data[0].position = vec3(-size / 2.0f, -size / 2.0f, size / 2.0f);
		data[1].position = vec3(size / 2.0f, -size / 2.0f, size / 2.0f);
		data[2].position = vec3(size / 2.0f, size / 2.0f, size / 2.0f);
		data[3].position = vec3(-size / 2.0f, size / 2.0f, size / 2.0f);
		data[4].position = vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
		data[5].position = vec3(size / 2.0f, -size / 2.0f, -size / 2.0f);
		data[6].position = vec3(size / 2.0f, size / 2.0f, -size / 2.0f);
		data[7].position = vec3(-size / 2.0f, size / 2.0f, -size / 2.0f);

		data[0].normal = vec3(-1.0f, -1.0f, 1.0f);
		data[1].normal = vec3(1.0f, -1.0f, 1.0f);
		data[2].normal = vec3(1.0f, 1.0f, 1.0f);
		data[3].normal = vec3(-1.0f, 1.0f, 1.0f);
		data[4].normal = vec3(-1.0f, -1.0f, -1.0f);
		data[5].normal = vec3(1.0f, -1.0f, -1.0f);
		data[6].normal = vec3(1.0f, 1.0f, -1.0f);
		data[7].normal = vec3(-1.0f, 1.0f, -1.0f);

		API::VertexBuffer* buffer = API::VertexBuffer::Create(API::BufferUsage::STATIC);
		buffer->SetData(8 * sizeof(Vertex), data);

		API::BufferLayout layout;
		layout.Push<vec3>("position");
		layout.Push<vec3>("normal");
		layout.Push<vec2>("uv");
		buffer->SetLayout(layout);

		API::VertexArray* va = API::VertexArray::Create();
		va->Bind();
		va->PushBuffer(buffer);
	
		uint* indices = new uint[36]
		{
			0, 1, 2, 2, 3, 0,
			3, 2, 6, 6, 7, 3,
			7, 6, 5, 5, 4, 7,
			4, 0, 3, 3, 7, 4,
			0, 1, 5, 5, 4, 0,
			1, 5, 6, 6, 2, 1
		};

		API::IndexBuffer* ib = API::IndexBuffer::Create(indices, 36);
		return new Mesh(va, ib, material);
	}

	Mesh* CreatePlane(float width, float height, const maths::vec3& normal, MaterialInstance* material)
	{
		using namespace maths;

		vec3 vec = normal * 90.0f;
		mat4 rotation = mat4::Rotate(vec.z, vec3(1, 0, 0)) * mat4::Rotate(vec.y, vec3(0, 1, 0)) * mat4::Rotate(vec.x, vec3(0, 0, 1));

		Vertex data[4];
		memset(data, 0, 4 * sizeof(Vertex));

		data[0].position = rotation * vec3(-width / 2.0f, 0.0f, -height / 2.0f);
		data[0].normal = normal;
		data[0].uv = vec2(0.0f, 0.0f);
		data[0].binormal = mat4::Rotate(90.0f, vec3(0, 1, 0)) * normal;
		data[0].tangent = mat4::Rotate(90.0f, vec3(0, 0, 1)) * normal;

		data[1].position = rotation * vec3(-width / 2.0f, 0.0f,  height / 2.0f);
		data[1].normal = normal;
		data[1].uv = vec2(0.0f, 1.0f);
		data[1].binormal = mat4::Rotate(90.0f, vec3(0, 1, 0)) * normal;
		data[1].tangent = mat4::Rotate(90.0f, vec3(0, 0, 1)) * normal;

		data[2].position = rotation * vec3( width / 2.0f, 0.0f,  height / 2.0f);
		data[2].normal = normal;
		data[2].uv = vec2(1.0f, 1.0f);
		data[2].binormal = mat4::Rotate(90.0f, vec3(0, 1, 0)) * normal;
		data[2].tangent = mat4::Rotate(90.0f, vec3(0, 0, 1)) * normal;

		data[3].position = rotation * vec3( width / 2.0f, 0.0f, -height / 2.0f);
		data[3].normal = normal;
		data[3].uv = vec2(1.0f, 0.0f);
		data[3].binormal = mat4::Rotate(90.0f, vec3(0, 1, 0)) * normal;
		data[3].tangent = mat4::Rotate(90.0f, vec3(0, 0, 1)) * normal;

		API::VertexBuffer* buffer = API::VertexBuffer::Create(API::BufferUsage::STATIC);
		buffer->SetData(8 * sizeof(Vertex), data);

		API::BufferLayout layout;
		layout.Push<vec3>("POSITION");
		layout.Push<vec3>("NORMAL");
		layout.Push<vec2>("TEXCOORD");
		layout.Push<vec3>("BINORMAL");
		layout.Push<vec3>("TANGENT");
		buffer->SetLayout(layout);

		API::VertexArray* va = API::VertexArray::Create();
		va->PushBuffer(buffer);

		uint* indices = new uint[6]
		{
			0, 1, 2,
			2, 3, 0
		};

		API::IndexBuffer* ib = API::IndexBuffer::Create(indices, 6);
		return new Mesh(va, ib, material);
	}

} } }