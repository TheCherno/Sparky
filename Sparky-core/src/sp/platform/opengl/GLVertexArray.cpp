#include "sp/sp.h"
#include "GLVertexArray.h"

#include <GL/glew.h>
#include "sp/utils/Log.h"

namespace sp { namespace graphics { namespace API {

	GLVertexArray::GLVertexArray()
	{
	}

	GLVertexArray::~GLVertexArray()
	{
	}

	void GLVertexArray::PushBuffer(API::VertexBuffer* buffer)
	{
		m_Buffers.push_back(buffer);
	}

	void GLVertexArray::Bind() const
	{
		m_Buffers.front()->Bind();
	}

	void GLVertexArray::Unbind() const
	{
		m_Buffers.front()->Unbind();
	}

	void GLVertexArray::Draw(uint count) const
	{
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL));
	}

} } }
