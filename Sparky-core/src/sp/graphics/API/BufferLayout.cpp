#include "sp/sp.h"
#include "BufferLayout.h"

namespace sp { namespace graphics { namespace API {

	BufferLayout::BufferLayout()
		: m_Size(0)
	{
	}

	void BufferLayout::Push(const String& name, uint type, uint size, uint count, bool normalized)
	{
		m_Layout.push_back({ name, type, size, count, m_Size, normalized });
		m_Size += size * count;
	}

	template<>
	void BufferLayout::Push<float>(const String& name, uint count, bool normalized)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(float), count, normalized);
				break;
			case RenderAPI::DIRECT3D:
				Push(name, DX_TYPE_R32_FLOAT, sizeof(float), count, normalized);
				break;
		}
	}

	template<>
	void BufferLayout::Push<uint>(const String& name, uint count, bool normalized)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
				Push(name, GL_UNSIGNED_INT, sizeof(uint), count, normalized);
				break;
			case RenderAPI::DIRECT3D:
				Push(name, DX_TYPE_R32_UINT, sizeof(uint), count, normalized);
				break;
		}
	}

	template<>
	void BufferLayout::Push<byte>(const String& name, uint count, bool normalized)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
				Push(name, GL_UNSIGNED_BYTE, sizeof(byte), count, normalized);
				break;
			case RenderAPI::DIRECT3D:
				Push(name, DX_TYPE_R8G8B8A8_UNORM, sizeof(byte) * 4, 1, normalized);
				break;
		}
	}

	template<>
	void BufferLayout::Push<maths::vec2>(const String& name, uint count, bool normalized)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(float), 2, normalized);
				break;
			case RenderAPI::DIRECT3D:
				Push(name, DX_TYPE_R32G32_FLOAT, sizeof(maths::vec2), count, normalized);
				break;
		}
	}

	template<>
	void BufferLayout::Push<maths::vec3>(const String& name, uint count, bool normalized)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(float), 3, normalized);
				break;
			case RenderAPI::DIRECT3D:
				Push(name, DX_TYPE_R32G32B32_FLOAT, sizeof(maths::vec3), count, normalized);
				break;
		}
	}

	template<>
	void BufferLayout::Push<maths::vec4>(const String& name, uint count, bool normalized)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(float), 4, normalized);
				break;
			case RenderAPI::DIRECT3D:
				Push(name, DX_TYPE_R32G32B32A32_FLOAT, sizeof(maths::vec4), count, normalized);
				break;
		}
	}


} } }