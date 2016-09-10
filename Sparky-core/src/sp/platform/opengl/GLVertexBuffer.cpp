#include "sp/sp.h"
#include "GLVertexBuffer.h"

#include "gl.h"
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
		: m_Usage(usage), m_ScratchBuffer(nullptr)
	{
		GLCall(glGenBuffers(1, &m_Handle));
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_Handle));
		if (m_ScratchBuffer)
			delete m_ScratchBuffer;
	}

	void GLVertexBuffer::Resize(uint size)
	{
		m_Size = size;
		if (m_ScratchBuffer)
			delete m_ScratchBuffer;
		m_ScratchBuffer = new byte[size];

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
#ifdef SP_PLATFORM_WIN32
		GLCall(void* result = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
#else
		void* result = m_ScratchBuffer;
#endif	
		return result;
	}

	void GLVertexBuffer::ReleasePointer()
	{
#ifdef SP_PLATFORM_WIN32
		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
#else
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, m_ScratchBuffer));
#endif
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
