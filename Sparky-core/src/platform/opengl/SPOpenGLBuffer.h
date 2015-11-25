#pragma once

#include <sparky_types.h>
#include <graphics/buffers/BufferLayout.h>

namespace sparky { namespace graphics { namespace API {

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
		T* GetPointer(uint flags = GL_WRITE_ONLY)
		{
			return (T*)API::GetBufferPointer(target, flags);
		}

		void ReleasePointer();

		void Bind();
		void Unbind();
	};

} } }