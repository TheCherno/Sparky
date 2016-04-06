#include "sp/sp.h"
#include "DXTextureDepth.h"

namespace sp { namespace graphics { namespace API {

	D3DTextureDepth::D3DTextureDepth(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	D3DTextureDepth::~D3DTextureDepth()
	{
		// GLCall(glDeleteTextures(1, &m_Handle));
	}

	void D3DTextureDepth::Bind(uint slot) const
	{
		// GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
	}

	void D3DTextureDepth::Unbind(uint slot) const
	{

	}

	void D3DTextureDepth::Init()
	{

	}

} } }