#include "sp/sp.h"
#include "Shader.h"

#include "sp/platform/opengl/GLShader.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXShader.h"
#endif

#include "sp/graphics/API/Context.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	const Shader* Shader::s_CurrentlyBound = nullptr;

	Shader* Shader::CreateFromFile(const String& name, const String& filepath, void* address)
	{
		String source = VFS::Get()->ReadTextFile(filepath);

		// TODO: Fix dynamic shader reloading
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:
			{
				GLShader* result = address ? new(address) GLShader(name, source) : spnew GLShader(name, source);
				result->m_Path = filepath;
				return result;
			}
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:
			{
				D3DShader* result = address ? new(address) D3DShader(name, source) : spnew D3DShader(name, source);
				result->m_FilePath = filepath;
				return result;
			}
#endif
		}
		return nullptr;
	}

	Shader* Shader::CreateFromSource(const String& name, const String& source)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLShader(name, source);
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return spnew D3DShader(name, source);
#endif
		}
		return nullptr;
	}

	bool Shader::TryCompile(const String& source, String& error)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return GLShader::TryCompile(source, error);
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return D3DShader::TryCompile(source, error);
#endif
		}
		return nullptr;
	}

	bool Shader::TryCompileFromFile(const String& filepath, String& error)
	{
		String source = VFS::Get()->ReadTextFile(filepath);
		return TryCompile(source, error);
	}

} } }