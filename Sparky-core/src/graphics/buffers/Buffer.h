#pragma once

#include <sparky_types.h>
#include <sparkygl.h>

namespace sparky { namespace graphics {

	class Buffer
	{
	private:
		uint m_BufferID;
		uint m_ComponentCount;
	public:
		Buffer(float* data, uint count, uint componentCount);
		~Buffer();
		void Bind() const;
		void Unbind() const;

		inline GLuint GetComponentCount() const { return m_ComponentCount; }
	};

} }