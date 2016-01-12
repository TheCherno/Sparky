#include "sp/sp.h"
#include "SPOpenGLBuffer.h"

#include <sp/graphics/SPRenderAPI.h>

namespace sp { namespace graphics { namespace API {

	Buffer::Buffer(uint target, uint usage)
		: target(target), usage(usage)
	{
		id = CreateBuffer();
	}

	Buffer::~Buffer()
	{
		FreeBuffer(id);
	}

	void Buffer::Resize(uint size)
	{
		this->size = size;
		SetBufferData(target, size, NULL, usage);
	}

	void Buffer::SetData(uint size, const void* data)
	{
		SetBufferData(target, size, data, usage);
	}

	void Buffer::ReleasePointer()
	{
		API::ReleaseBufferPointer(target);
	}

	void Buffer::Bind()
	{
		BindBuffer(target, id);
	}

	void Buffer::Unbind()
	{
		UnbindBuffers(target);
	}

} } }