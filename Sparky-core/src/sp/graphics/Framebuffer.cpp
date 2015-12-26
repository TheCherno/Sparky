#include "sp/sp.h"
#include "Framebuffer.h"

#include "sp/graphics/SPRenderAPI.h"

namespace sp { namespace graphics {

	Framebuffer::Framebuffer(const maths::tvec2<uint>& size)
		: m_Size(size), m_Width(m_Size.x), m_Height(m_Size.y), m_ClearColor(maths::vec4(0.0f, 0.0f, 0.0f, 0.0f))
	{
		Create(m_Width, m_Height);
	}

	Framebuffer::Framebuffer(uint width, uint height)
		: m_Size(width, height), m_Width(m_Size.x), m_Height(m_Size.y), m_ClearColor(maths::vec4(0.0f, 0.0f, 0.0f, 0.0f))
	{
		Create(width, height);
	}

	Framebuffer::~Framebuffer()
	{
		API::FreeFramebuffer(m_Data.framebufferID);
	}

	void Framebuffer::Create(uint width, uint height)
	{
		m_Data.framebufferID = API::CreateFramebuffer();
		m_Data.depthbufferID = API::CreateRenderbuffer();
		
		Texture::SetFilter(TextureFilter::LINEAR);
		m_Texture = new Texture(width, height, 32);

		API::BindRenderbuffer(GL_RENDERBUFFER, m_Data.depthbufferID);
		API::RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

		API::BindFramebuffer(GL_FRAMEBUFFER, m_Data.framebufferID);
		API::FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetID(), 0);
		API::FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Data.depthbufferID);
		API::BindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind() const
	{
		API::BindFramebuffer(GL_FRAMEBUFFER, m_Data.framebufferID);
		API::SetViewport(0, 0, m_Width, m_Height);
	}

	void Framebuffer::Unbind() const
	{
		API::BindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Clear()
	{
		API::SetClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
		API::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

} }