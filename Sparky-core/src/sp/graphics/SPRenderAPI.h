#pragma once

#include <sp/Types.h>

namespace sp { namespace graphics { namespace API {

	// SPRenderAPI is a low-level platform-independent and api-independent rendering API.
	// This should contain the lowest-level functions. The VertexBuffer, IndexBuffer,
	// VertexArray, and Renderer classes contain higher-level implementations which should
	// be used whenever possible.

	// This API will be extended and CHANGED when Direct3D and other rendering APIs are
	// added to Sparky, so this API is definitely not safe.

	inline uint CreateVertexArray();
	inline uint CreateBuffer();
	inline uint CreateFramebuffer();
	inline uint CreateRenderbuffer();

	inline void CreateVertexArrays(uint size, uint* arrays);
	inline void CreateBuffers(uint size, uint* buffers);
	inline void CreateFramebuffers(uint size, uint* buffers);
	inline void CreateRenderbuffers(uint size, uint* buffers);

	inline void BindVertexArray(uint array);
	inline void BindBuffer(uint target, uint buffer);
	inline void BindFramebuffer(uint target, uint buffer);
	inline void BindRenderbuffer(uint target, uint buffer);

	inline void SetBufferData(uint target, uint size, const void* data, uint usage);
	inline void SetBufferSubData(uint target, ptrdiff_t offset, ptrdiff_t size, const void* data);
	inline void* GetBufferPointer(uint target, uint access);
	inline void ReleaseBufferPointer(uint target);

	inline void FramebufferTexture2D(uint target, uint attachment, uint textarget, uint texture, int level);
	inline void FramebufferRenderbuffer(uint target, uint attachment, uint renderbuffertarget, uint renderbuffer);
	inline void RenderbufferStorage(uint target, uint format, uint width, uint height);
	inline int GetScreenBuffer();

	inline void SetViewport(uint x, uint y, uint width, uint height);
	inline void Clear(uint flags);
	inline void SetClearColor(float r, float g, float b, float a);

	inline uint CreateTexture();
	inline void CreateTextures(uint size, uint* textures);
	inline void BindTexture(uint target, uint texture);
	inline void UnbindTexture(uint target);
	inline void SetTextureParameter(uint target, uint parameter, int value);
	inline void SetTextureData(uint target, uint internalformat, uint width, uint height, uint format, uint type, const void* pixels);
	inline void SetActiveTexture(uint texture);

	inline void UnbindVertexArrays();
	inline void UnbindBuffers(uint target);

	inline void EnableVertexAttribute(uint index);
	inline void DisableVertexAttribute(uint index);
	inline void SetVertexAttributePointer(uint index, int size, uint type, bool normalized, uint stride, uint offset);

	inline void FreeVertexArray(uint array);
	inline void FreeBuffer(uint buffer);
	inline void FreeFramebuffer(uint buffer);

	inline void FreeVertexArrays(uint size, uint* arrays);
	inline void FreeBuffers(uint size, uint* buffers);
	inline void FreeFramebuffers(uint size, uint* buffers);

	inline void FreeTexture(uint texture);
	inline void FreeTextures(uint size, uint* textures);

	inline void DrawElements(uint mode, uint count, uint type, const void* indices);

} } }

#ifdef SP_PLATFORM_WINDOWS
	#include "sp/platform/opengl/SPOpenGLRenderAPI.inl"
	#include "sp/platform/opengl/SPOpenGLBuffer.h"
#endif