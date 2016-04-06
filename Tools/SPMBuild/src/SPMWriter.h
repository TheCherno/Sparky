#pragma once

#include <iostream>
#include <vector>

#include "Utils.h"

namespace sp { namespace tools {

	enum VertexAttribute
	{
		POSITION	= BIT(0),
		NORMAL		= BIT(1),
		UV			= BIT(2),
		BINORMAL	= BIT(3),
		TANGENT		= BIT(4),
	};

	struct SPMFormat
	{
		char* header = "SPMF";
		byte nameLength;
		char* name;
		// TODO: Multiple objects / meshes / submeshes
		// TODO: Index into these things ^
		// TODO: Properties (eg. transform)
		// TODO: Objects / meshes should have a material index
		uint attributes; // Bitfield
		uint vertexBufferSize;
		byte* vertexData;
		uint indexBufferSize;
		byte* indexData;
		// TODO: Materials
		char* footer = "1234";
	};

	class SPMWriter
	{
	private:
		String m_Name;
		uint m_Attributes;
		const std::vector<Vertex>& m_VertexBuffer;
		const std::vector<uint>& m_IndexBuffer;
		SPMFormat m_Format;
	public:
		SPMWriter(const String& name, uint attributes, const std::vector<Vertex>& vertices, const std::vector<uint>& indices);
		~SPMWriter();

		void Write(const String& file);
	};

} }
