#include "FontManager.h"

namespace sp { namespace graphics {

	std::vector<Font*> FontManager::m_Fonts;

	void FontManager::Add(Font* font)
	{
		m_Fonts.push_back(font);
	}

	Font* FontManager::Get()
	{
		return m_Fonts[0];
	}

	Font* FontManager::Get(const String& name)
	{
		for (Font* font : m_Fonts)
		{
			if (font->GetName() == name)
				return font;
		}
		// TODO: Maybe return a default font instead?
		return nullptr;
	}

	Font* FontManager::Get(const String& name, unsigned int size)
	{
		for (Font* font : m_Fonts)
		{
			if (font->GetSize() == size && font->GetName() == name)
				return font;
		}
		// TODO: Maybe return a default font instead?
		return nullptr;
	}

	void FontManager::Clean()
	{
		for (uint i = 0; i < m_Fonts.size(); i++)
			delete m_Fonts[i];
	}

} }