#pragma once

#include <sparky_types.h>
#include <sparkygl.h>

namespace sparky { namespace graphics {

	class IndexBuffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_Count;
	public:
		IndexBuffer(short* data, uint count);
		IndexBuffer(uint* data, uint count);
		~IndexBuffer();
		void Bind() const;
		void Unbind() const;

		inline GLuint GetCount() const { return m_Count; }
	};

} }