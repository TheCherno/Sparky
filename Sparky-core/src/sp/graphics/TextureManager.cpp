#include "sp/sp.h"
#include "sp/Common.h"

#include "TextureManager.h"

namespace sp { namespace graphics {

	std::vector<API::Texture*> TextureManager::m_Textures;

	API::Texture* TextureManager::Add(API::Texture* texture)
	{
		m_Textures.push_back(texture);
		return texture;
	}

	API::Texture* TextureManager::Get(const String& name)
	{
		for (API::Texture* texture : m_Textures)
		{
			if (texture->GetName() == name)
				return texture;
		}
		return nullptr;
	}

	void TextureManager::Clean()
	{
		for (uint i = 0; i < m_Textures.size(); i++)
			delete m_Textures[i];
	}

} }