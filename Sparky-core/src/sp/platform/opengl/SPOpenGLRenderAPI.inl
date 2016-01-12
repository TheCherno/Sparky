#include "sp/utils/Log.h"

#ifdef SPARKY_PLATFORM_WEB
	#define GLFW_INCLUDE_ES3
#else
	#include <GL/glew.h>
#endif

#define SP_FLOAT GL_FLOAT
#define SP_UNSIGNED_SHORT GL_UNSIGNED_SHORT

namespace sp { namespace graphics { namespace API {

	inline uint CreateVertexArray()
	{
		uint result;
		GLCall(glGenVertexArrays(1, &result));
		return result;
	}

	inline uint CreateBuffer()
	{
		uint result;
		GLCall(glGenBuffers(1, &result));
		return result;
	}

	inline uint CreateFramebuffer()
	{
		uint result;
		GLCall(glGenFramebuffers(1, &result));
		return result;
	}

	inline uint CreateRenderbuffer()
	{
		uint result;
		GLCall(glGenRenderbuffers(1, &result));
		return result;
	}

	inline void CreateVertexArrays(uint size, uint* arrays)
	{
		GLCall(glGenVertexArrays(size, arrays));
	}

	inline void CreateBuffers(uint size, uint* buffers)
	{
		GLCall(glGenBuffers(size, buffers));
	}

	inline void CreateFramebuffers(uint size, uint* buffers)
	{
		GLCall(glGenFramebuffers(size, buffers));
	}

	inline void CreateRenderbuffers(uint size, uint* buffers)
	{
		GLCall(glGenRenderbuffers(size, buffers));
	}

	inline void BindVertexArray(uint array)
	{
		GLCall(glBindVertexArray(array));
	}

	inline void BindBuffer(uint target, uint buffer)
	{
		GLCall(glBindBuffer(target, buffer));
	}

	inline void BindFramebuffer(uint target, uint buffer)
	{
		GLCall(glBindFramebuffer(target, buffer));
	}

	inline void BindRenderbuffer(uint target, uint buffer)
	{
		GLCall(glBindRenderbuffer(target, buffer));
	}

	inline int GetScreenBuffer()
	{
		int result;
		GLCall(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result));
		return result;
	}

	inline void SetBufferData(uint target, uint size, const void* data, uint usage)
	{
		GLCall(glBufferData(target, size, data, usage));
	}

	inline void SetBufferSubData(uint target, ptrdiff_t offset, ptrdiff_t size, const void* data)
	{
		GLCall(glBufferSubData(target, offset, size, data));
	}

	inline void* GetBufferPointer(uint target, uint access)
	{
		GLCall(void* result = glMapBuffer(target, access));
		return result;
	}

	inline void ReleaseBufferPointer(uint target)
	{
		GLCall(glUnmapBuffer(target));
	}

	inline void FramebufferTexture2D(uint target, uint attachment, uint textarget, uint texture, int level)
	{
		GLCall(glFramebufferTexture2D(target, attachment, textarget, texture, level));
	}

	inline void FramebufferRenderbuffer(uint target, uint attachment, uint renderbuffertarget, uint renderbuffer)
	{
		GLCall(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));
	}

	inline void RenderbufferStorage(uint target, uint format, uint width, uint height)
	{
		GLCall(glRenderbufferStorage(target, format, width, height));
	}

	inline void SetViewport(uint x, uint y, uint width, uint height)
	{
		GLCall(glViewport(x, y, width, height));
	}

	inline void Clear(uint flags)
	{
		GLCall(glClear(flags));
	}

	inline void SetClearColor(float r, float g, float b, float a)
	{
		GLCall(glClearColor(r, g, b, a));
	}

	inline uint CreateTexture()
	{
		GLuint result;
		GLCall(glGenTextures(1, &result));
		return result;
	}

	inline void CreateTextures(uint size, uint* textures)
	{
		GLCall(glGenTextures(size, textures));
	}

	inline void BindTexture(uint target, uint texture)
	{
		GLCall(glBindTexture(target, texture));
	}

	inline void UnbindTexture(uint target)
	{
		GLCall(glBindTexture(target, 0));
	}

	inline void SetTextureParameter(uint target, uint parameter, int value)
	{
		GLCall(glTexParameteri(target, parameter, value));
	}

	inline void SetTextureData(uint target, uint internalformat, uint width, uint height, uint format, uint type, const void* pixels)
	{
		GLCall(glTexImage2D(target, 0, internalformat, width, height, 0, format, type, pixels));
	}

	inline void SetActiveTexture(uint texture)
	{
		GLCall(glActiveTexture(texture));
	}

	inline void UnbindVertexArrays()
	{
		GLCall(glBindVertexArray(0));
	}

	inline void UnbindBuffers(uint target)
	{
		GLCall(glBindBuffer(target, 0));
	}

	inline void EnableVertexAttribute(uint index)
	{
		GLCall(glEnableVertexAttribArray(index));
	}

	inline void DisableVertexAttribute(uint index)
	{
		GLCall(glDisableVertexAttribArray(index));
	}

	inline void SetVertexAttributePointer(uint index, int size, uint type, bool normalized, uint stride, uint offset)
	{
		GLCall(glVertexAttribPointer(index, size, type, (uint)normalized, stride, (const GLvoid*)offset));
	}

	inline void FreeArray(uint array)
	{
		GLCall(glDeleteVertexArrays(1, &array));
	}

	inline void FreeBuffer(uint buffer)
	{
		GLCall(glDeleteBuffers(1, &buffer));
	}

	inline void FreeFramebuffer(uint buffer)
	{
		GLCall(glDeleteFramebuffers(1, &buffer));
	}

	inline void FreeVertexArray(uint array)
	{
		GLCall(glDeleteVertexArrays(1, &array));
	}

	inline void FreeVertexArrays(uint size, uint* arrays)
	{
		GLCall(glDeleteVertexArrays(size, arrays));
	}

	inline void FreeBuffers(uint size, uint* buffers)
	{
		GLCall(glDeleteBuffers(size, buffers));
	}

	inline void FreeFramebuffers(uint size, uint* buffers)
	{
		GLCall(glDeleteFramebuffers(size, buffers));
	}

	inline void FreeTexture(uint texture)
	{
		GLCall(glDeleteTextures(1, &texture));
	}

	inline void FreeTextures(uint size, uint* textures)
	{
		GLCall(glDeleteTextures(size, textures));
	}

	inline void DrawElements(uint mode, uint count, uint type, const void* indices)
	{
		GLCall(glDrawElements(mode, count, type, indices));
	}

} } }