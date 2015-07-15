#include "indexbuffer.h"

namespace sparky { namespace graphics {

	IndexBuffer::IndexBuffer(GLushort* data, GLsizei count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::IndexBuffer(GLuint* data, GLsizei count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_BufferID));
	}

	void IndexBuffer::bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	}

	void IndexBuffer::unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


} }