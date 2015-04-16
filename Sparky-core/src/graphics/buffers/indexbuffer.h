#pragma once

#ifdef SPARKY_EMSCRIPTEN
	#define GLFW_INCLUDE_ES3
	#include <GLFW/glfw3.h>
#else
	#include <GL/glew.h>
#endif

namespace sparky { namespace graphics {

	class IndexBuffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_Count;
	public:
		IndexBuffer(GLushort* data, GLsizei count);
		IndexBuffer(GLuint* data, GLsizei count);
		~IndexBuffer();
		void bind() const;
		void unbind() const;

		inline GLuint getCount() const { return m_Count; }
	};

} }