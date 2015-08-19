#include "IndexBuffer.h"

namespace sparky { namespace graphics {

	IndexBuffer::IndexBuffer(short* data, uint count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(short), data, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::IndexBuffer(uint* data, uint count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_BufferID));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


} }