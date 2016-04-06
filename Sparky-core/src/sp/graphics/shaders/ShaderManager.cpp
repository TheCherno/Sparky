#include "sp/sp.h"
#include "ShaderManager.h"

#include "sp/system/Memory.h"

namespace sp { namespace graphics {

	std::vector<API::Shader*> ShaderManager::s_Shaders;

	void ShaderManager::Add(API::Shader* shader)
	{
		s_Shaders.push_back(shader);
	}

	API::Shader* ShaderManager::Get(const String& name)
	{
		for (API::Shader* shader : s_Shaders)
		{
			if (shader->GetName() == name)
				return shader;
		}
		SP_ASSERT(false, "Could not find '", name, "' shader!");
		return nullptr;
	}

	void ShaderManager::Clean()
	{
		for (uint i = 0; i < s_Shaders.size(); i++)
			spdel s_Shaders[i];
	}

	void ShaderManager::Reload(const String& name)
	{
		for (uint i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i]->GetName() == name)
			{
				String path = s_Shaders[i]->GetFilePath();
				String error;
				if (!API::Shader::TryCompileFromFile(path, error))
				{
					SP_ERROR(error);
				}
				else
				{
					s_Shaders[i]->~Shader();
					s_Shaders[i] = API::Shader::CreateFromFile(name, path, s_Shaders[i]);
				}
				return;
			}
		}
		SP_WARN("Could not find '", name, "' shader to reload.");
	}

	void ShaderManager::Reload(const API::Shader* shader)
	{
		for (uint i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i] == shader)
			{
				String name = shader->GetName();
				String path = shader->GetFilePath();
				s_Shaders[i]->~Shader();
				s_Shaders[i] = API::Shader::CreateFromFile(name, path, s_Shaders[i]);
				return;
			}
		}
		SP_WARN("Could not find specified shader to reload.");
	}


} }