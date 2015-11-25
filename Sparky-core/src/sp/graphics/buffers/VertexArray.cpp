#include "VertexArray.h"

namespace sp { namespace graphics {

	uint VertexArray::s_CurrentBinding = 0;

	VertexArray::VertexArray()
	{
		m_ID = API::CreateVertexArray();
	}

	VertexArray::~VertexArray()
	{
		for (uint i = 0; i < m_Buffers.size(); i++)
			delete m_Buffers[i];

		API::FreeVertexArray(m_ID);
	}


	API::Buffer* VertexArray::GetBuffer(uint index)
	{
		return m_Buffers[index];
	}

	void VertexArray::PushBuffer(API::Buffer* buffer)
	{
		SPARKY_ASSERT(s_CurrentBinding == m_ID);

		m_Buffers.push_back(buffer);

		const std::vector<BufferLayoutType>& layout = buffer->layout.GetLayout();
		for (uint i = 0; i < layout.size(); i++)
		{
			API::EnableVertexAttribute(i);
			API::SetVertexAttributePointer(i, layout[i].count, layout[i].type, layout[i].normalized, buffer->layout.GetStride(), layout[i].offset);
		}
	}

	void VertexArray::Bind() const
	{
#ifdef SP_DEBUG
		s_CurrentBinding = m_ID;
#endif
		API::BindVertexArray(m_ID);
	}

	void VertexArray::Unbind() const
	{
#ifdef SP_DEBUG
		s_CurrentBinding = 0;
#endif
		API::UnbindVertexArrays();
	}

} }