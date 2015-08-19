#include "Framebuffer.h"

namespace sparky { namespace graphics {

	Framebuffer::Framebuffer(const maths::tvec2<uint>& size)
		: m_Size(size), m_Width(m_Size.x), m_Height(m_Size.y)
	{
		Create(m_Width, m_Height);
	}

	Framebuffer::Framebuffer(uint width, uint height)
		: m_Size(width, height), m_Width(m_Size.x), m_Height(m_Size.y)
	{
		Create(width, height);
	}

	Framebuffer::~Framebuffer()
	{
		GLCall(glDeleteFramebuffers(1, &m_Data.framebufferID));
	}

	void Framebuffer::Create(uint width, uint height)
	{
		GLCall(glGenFramebuffers(1, &m_Data.framebufferID));
		GLCall(glGenRenderbuffers(1, &m_Data.depthbufferID));
		
		m_Texture = new Texture(width, height);

		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_Data.depthbufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Data.framebufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetID(), 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Data.depthbufferID));
	}

	void Framebuffer::Bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Data.framebufferID));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void Framebuffer::Clear()
	{
		GLCall(glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

} }