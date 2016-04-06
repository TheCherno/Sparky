#include "sp/sp.h"
#include "DXFramebuffer2D.h"

#include "sp/system/Memory.h"
#include "sp/utils/Log.h"

namespace sp { namespace graphics {

	using namespace API;

	D3DFramebuffer2D::D3DFramebuffer2D(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	D3DFramebuffer2D::~D3DFramebuffer2D()
	{
		
	}

	void D3DFramebuffer2D::Init()
	{

	}

	void D3DFramebuffer2D::Bind() const
	{

	}

	void D3DFramebuffer2D::Unbind() const
	{

	}

	void D3DFramebuffer2D::Clear()
	{

	}

} }