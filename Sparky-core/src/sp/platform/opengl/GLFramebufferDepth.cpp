#include "sp/sp.h"
#include "GLFramebufferDepth.h"

#include "sp/system/Memory.h"

#include "gl.h"
#include "sp/utils/Log.h"

namespace sp { namespace graphics {

	GLFramebufferDepth::GLFramebufferDepth(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	GLFramebufferDepth::~GLFramebufferDepth()
	{
		GLCall(glDeleteFramebuffers(1, &m_FramebufferID));
	}

	void GLFramebufferDepth::Init()
	{
		GLCall(glGenFramebuffers(1, &m_FramebufferID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));

		m_Texture = spnew API::GLTextureDepth(m_Width, m_Height);

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ((API::GLTextureDepth*)m_Texture)->GetHandle(), 0));
#ifdef SP_PLATFORM_WIN32
		GLCall(glDrawBuffer(GL_NONE));
#endif
		SP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFramebufferDepth::Bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void GLFramebufferDepth::Unbind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		// GLCall(glViewport(0, 0, Application::GetApplication().GetWidth(), Application::GetApplication().GetHeight())); // TODO: Temporary
	}

	void GLFramebufferDepth::Clear()
	{
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	}

} }