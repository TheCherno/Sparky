#pragma once

#include <vector>
#include <sp/Types.h>

#include "Font.h"

namespace sp { namespace graphics {

	class FontManager
	{
	private:
		static std::vector<Font*> m_Fonts;
	public:
		static void Add(Font* font);
		static Font* Get();
		static Font* Get(const String& name);
		static Font* Get(const String& name, unsigned int size);
		static void Clean();
	private:
		FontManager() { }
	};

} }