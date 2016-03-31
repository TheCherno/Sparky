#include "sp/sp.h"

#include "GBuffer.h"

namespace sp { namespace graphics {

#if 0
	GBuffer::GBuffer(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	GBuffer::~GBuffer()
	{
		API::FreeFramebuffer(m_Framebuffer);
		API::FreeTextures(4, m_Textures);
		API::FreeTexture(m_DepthTexture);
	}

	void GBuffer::Init()
	{
		m_Framebuffer = API::CreateFramebuffer();
		API::BindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

		m_DepthTexture = API::CreateTexture();
		for (uint i = 0; i < 4; i++)
		{
			m_Textures[i] = API::CreateTexture();
			API::BindTexture(GL_TEXTURE_2D, m_Textures[i]);
			API::SetTextureData(GL_TEXTURE_2D, GL_RGBA, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			API::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Textures[i], 0);
		}

		API::BindTexture(GL_TEXTURE_2D, m_DepthTexture);
		API::SetTextureData(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32F, m_Width, m_Height, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		API::FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

		uint drawBuffers[4] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3
		};

		API::SetDrawBuffers(drawBuffers, 4);
		SP_ASSERT(API::CheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		API::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void GBuffer::Bind(int32 mode)
	{
		if (mode)
			API::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer);
		else
			API::BindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer);
	}

	void GBuffer::SetReadBuffer(TextureType type)
	{
		API::ReadBuffer(GL_COLOR_ATTACHMENT0 + (uint)type);
	}
#endif

} }