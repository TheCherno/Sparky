#include "IndexBuffer.h"

namespace sp { namespace graphics {

	IndexBuffer::IndexBuffer(short* data, uint count)
		: m_Count(count)
	{
		m_Buffer = new API::Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_Buffer->Bind();
		m_Buffer->SetData(count * sizeof(short), data);
		m_Buffer->Unbind();
	}

	IndexBuffer::IndexBuffer(uint* data, uint count)
		: m_Count(count)
	{
		m_Buffer = new API::Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_Buffer->Bind();
		m_Buffer->SetData(count * sizeof(uint), data);
		m_Buffer->Unbind();
	}

	IndexBuffer::~IndexBuffer()
	{
		delete m_Buffer;
	}

	void IndexBuffer::Bind() const
	{
		m_Buffer->Bind();
	}

	void IndexBuffer::Unbind() const
	{
		m_Buffer->Unbind();
	}

} }