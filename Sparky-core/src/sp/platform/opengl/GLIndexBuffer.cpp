#include "sp/sp.h"
#include "GLIndexBuffer.h"

#include <GL/glew.h>
#include "sp/utils/Log.h"

namespace sp { namespace graphics { namespace API {

	GLIndexBuffer::GLIndexBuffer(uint16* data, uint count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_Handle));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint16), data, GL_STATIC_DRAW));
	}

	GLIndexBuffer::GLIndexBuffer(uint* data, uint count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_Handle));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_Handle));
	}

	void GLIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
	}

	void GLIndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	uint GLIndexBuffer::GetCount() const
	{
		return m_Count;
	}

} } }