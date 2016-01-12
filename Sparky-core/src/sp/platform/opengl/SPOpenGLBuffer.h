#pragma once

#include <sp/Types.h>
#include <sp/graphics/buffers/BufferLayout.h>

namespace sp { namespace graphics { namespace API {

	struct Buffer
	{
		uint id;
		uint target, usage;
		uint size;
		BufferLayout layout;

		Buffer(uint target, uint usage);
		~Buffer();

		void Resize(uint size);
		void SetData(uint size, const void* data);

		template<typename T>
		T* GetPointer(uint flags = 0x88B9/*GL_WRITE_ONLY*/)
		{
			return (T*)API::GetBufferPointer(target, flags);
		}

		void ReleasePointer();

		void Bind();
		void Unbind();
	};

} } }