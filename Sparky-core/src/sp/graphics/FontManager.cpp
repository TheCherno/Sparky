#include "sp/sp.h"
#include "FontManager.h"

#include "sp/embedded/Embedded.h"

namespace sp { namespace graphics {

	std::vector<Font*> FontManager::s_Fonts; 
	maths::vec2 FontManager::s_Scale = maths::vec2(1, 1);

	void FontManager::SetScale(const maths::vec2& scale)
	{
		s_Scale = scale;
	}

	void FontManager::Add(Font* font)
	{
		font->SetScale(s_Scale);
		s_Fonts.push_back(font);
	}

	Font* FontManager::Get()
	{
		return s_Fonts[0];
	}

	Font* FontManager::Get(const String& name)
	{
		for (Font* font : s_Fonts)
		{
			if (font->GetName() == name)
				return font;
		}
		// TODO: Maybe return a default font instead?
		return nullptr;
	}

	Font* FontManager::Get(uint size)
	{
		for (Font* font : s_Fonts)
		{
			if (font->GetSize() == size)
				return font;
		}
		Font* result = new Font("SourceSansPro", internal::DEFAULT_FONT, internal::DEFAULT_FONT_SIZE, size);
		result->SetScale(s_Scale);
		Add(result);
		return result;
	}

	Font* FontManager::Get(const String& name, uint size)
	{
		for (Font* font : s_Fonts)
		{
			if (font->GetSize() == size && font->GetName() == name)
				return font;
		}
		// TODO: Maybe return a default font instead?
		return nullptr;
	}

	void FontManager::Clean()
	{
		for (uint i = 0; i < s_Fonts.size(); i++)
			delete s_Fonts[i];
	}

} }