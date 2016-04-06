#include "SPMWriter.h"

#include <stdlib.h>

namespace sp { namespace tools {

	SPMWriter::SPMWriter(const String& name, uint attributes, const std::vector<Vertex>& vertices, const std::vector<uint>& indices)
		: m_Name(name), m_Attributes(attributes), m_VertexBuffer(vertices), m_IndexBuffer(indices)
	{
		m_Format.nameLength = name.length();
		m_Format.name = &m_Name[0];
		m_Format.attributes = attributes;
		m_Format.vertexBufferSize = m_VertexBuffer.size() * sizeof(Vertex);
		m_Format.vertexData = (byte*)&m_VertexBuffer[0];
		m_Format.indexBufferSize = m_IndexBuffer.size() * sizeof(uint);
		m_Format.indexData = (byte*)&m_IndexBuffer[0];
	}

	SPMWriter::~SPMWriter()
	{
	}

	void WriteBytes(FILE* file, const byte* data, uint size)
	{
		fwrite(data, 1, size, file);
	}

	void SPMWriter::Write(const String& file)
	{
		const SPMFormat& format = m_Format;

		FILE* f = fopen(file.c_str(), "wb");
		WriteBytes(f, (byte*)format.header, 4);
		WriteBytes(f, &format.nameLength, 1);
		WriteBytes(f, (byte*)format.name, format.nameLength);
		WriteBytes(f, (byte*)&format.attributes, sizeof(uint));
		WriteBytes(f, (byte*)&format.vertexBufferSize, sizeof(uint));
		WriteBytes(f, format.vertexData, format.vertexBufferSize);
		WriteBytes(f, (byte*)&format.indexBufferSize, sizeof(uint));
		WriteBytes(f, format.indexData, format.indexBufferSize);
		WriteBytes(f, (byte*)format.footer, 4);
		fclose(f);
	}

} }