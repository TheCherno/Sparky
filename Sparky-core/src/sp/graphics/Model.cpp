#include "sp/sp.h"
#include "Model.h"

#include <GL/glew.h>

#include "sp/system/Memory.h"
#include "sp/graphics/shaders/ShaderManager.h"
#include "sp/system/VFS.h"

namespace sp { namespace graphics {

	using namespace maths;

	enum VertexAttribute
	{
		POSITION = BIT(0),
		NORMAL = BIT(1),
		UV = BIT(2),
		BINORMAL = BIT(3),
		TANGENT = BIT(4),
	};

	struct SPMFormat
	{
		char* header = "SPMF";
		byte nameLength;
		char* name;
		uint attributes;
		uint vertexBufferSize;
		byte* vertexData;
		uint indexBufferSize;
		byte* indexData;
		char* footer = "1234";
	};

	Model::Model(const String& path, MaterialInstance* materialInstance)
	{
		Load(path);
		m_Mesh->SetMaterial(materialInstance);
	}

	Model::~Model()
	{
		delete m_Mesh;
	}

	byte* ReadBytes(FILE* file, byte* buffer, uint size)
	{
		fread(buffer, 1, size, file);
		return buffer;
	}

	void Model::Load(const String& path)
	{
		// TODO: Change to VFS::OpenMemoryMap()
		String physicalPath;
		VFS::Get()->ResolvePhysicalPath(path, physicalPath);
		SPMFormat format;
		FILE* f = fopen(physicalPath.c_str(), "rb");
		SP_ASSERT(f);
		{
			byte header[4];
			ReadBytes(f, header, 4);
			SP_ASSERT(memcmp(header, format.header, 4) == 0);
		}

		{
			byte buffer[1];
			ReadBytes(f, buffer, 1);
			format.nameLength = *buffer;
		}

		{
			format.name = new char[format.nameLength + 1];
			ReadBytes(f, (byte*)format.name, format.nameLength);
			format.name[format.nameLength] = '\0';
		}

		{
			byte buffer[4];
			ReadBytes(f, buffer, 4);
			format.attributes = *(uint*)buffer;
		}

		{
			byte buffer[4];
			ReadBytes(f, buffer, 4);
			format.vertexBufferSize = *(uint*)buffer;
		}

		{
			format.vertexData = spnew byte[format.vertexBufferSize];
			ReadBytes(f, format.vertexData, format.vertexBufferSize);
		}

		{
			byte buffer[4];
			ReadBytes(f, buffer, 4);
			format.indexBufferSize = *(uint*)buffer;
		}

		{
			format.indexData = spnew byte[format.indexBufferSize];
			ReadBytes(f, format.indexData, format.indexBufferSize);
		}

		{
			byte footer[4];
			ReadBytes(f, footer, 4);
			SP_ASSERT(memcmp(footer, format.footer, 4) == 0);
		}

		fclose(f);

		ShaderManager::Get("AdvancedLighting")->Bind();

		API::VertexBuffer* buffer = API::VertexBuffer::Create(API::BufferUsage::STATIC);
		buffer->SetData(format.vertexBufferSize, format.vertexData);

		API::BufferLayout layout;
		layout.Push<vec3>("POSITION");
		layout.Push<vec3>("NORMAL");
		layout.Push<vec2>("TEXCOORD");
		layout.Push<vec3>("BINORMAL");
		layout.Push<vec3>("TANGENT");
		buffer->SetLayout(layout);

		API::VertexArray* va = API::VertexArray::Create();
		va->PushBuffer(buffer);

		API::IndexBuffer* ib = API::IndexBuffer::Create((uint*)format.indexData, format.indexBufferSize / sizeof(uint));
		m_Mesh = spnew Mesh(va, ib, nullptr);
	}

	void Model::Render(Renderer3D& renderer)
	{
		m_Mesh->Render(renderer);
	}

} }