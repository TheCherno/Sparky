#include "sp/sp.h"
#include "GLVertexBuffer.h"

#include <GL/glew.h>
#include "sp/utils/Log.h"

namespace sp { namespace graphics { namespace API {
	
	static uint SPBufferUsageToOpenGL(BufferUsage usage)
	{
		switch (usage)
		{
			case BufferUsage::STATIC: return GL_STATIC_DRAW;
			case BufferUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
		}
		return 0;
	}

	GLVertexBuffer::GLVertexBuffer(BufferUsage usage)
		: m_Usage(usage)
	{
		GLCall(glGenBuffers(1, &m_Handle));
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_Handle));
	}

	void GLVertexBuffer::Resize(uint size)
	{
		m_Size = size;

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, SPBufferUsageToOpenGL(m_Usage)));
	}

	void GLVertexBuffer::SetLayout(const BufferLayout& bufferLayout)
	{
		m_Layout = bufferLayout;
		const std::vector<BufferElement>& layout = bufferLayout.GetLayout();
		for (uint i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, bufferLayout.GetStride(), (const void*)element.offset));
		}
	}

	void GLVertexBuffer::SetData(uint size, const void* data)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, SPBufferUsageToOpenGL(m_Usage)));
	}

	void* GLVertexBuffer::GetPointerInternal()
	{
		GLCall(void* result = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		return result;
	}

	void GLVertexBuffer::ReleasePointer()
	{
		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
	}

	void GLVertexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		SetLayout(m_Layout);
	}

	void GLVertexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

} } }
