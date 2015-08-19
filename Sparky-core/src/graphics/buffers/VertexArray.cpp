#include "VertexArray.h"

namespace sparky { namespace graphics {

	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_ArrayID));
	}

	VertexArray::~VertexArray()
	{
		for (uint i = 0; i < m_Buffers.size(); i++)
			delete m_Buffers[i];

		GLCall(glDeleteVertexArrays(1, &m_ArrayID));
	}

	void VertexArray::AddBuffer(Buffer* buffer, uint index)
	{
		Bind();
		buffer->Bind();

		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(index, buffer->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, 0));

		buffer->Unbind();
		Unbind();

		m_Buffers.push_back(buffer);
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_ArrayID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

} }