#include "Buffer.h"

namespace sparky { namespace graphics {

	Buffer::Buffer(float* data, uint count, uint componentCount)
		: m_ComponentCount(componentCount)
	{
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	Buffer::~Buffer()
	{
		GLCall(glDeleteBuffers(1, &m_BufferID));
	}

	void Buffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
	}

	void Buffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


} }