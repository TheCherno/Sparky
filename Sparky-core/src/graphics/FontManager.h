#pragma once

#include <vector>
#include <sparky_types.h>

#include "Font.h"

namespace sparky { namespace graphics {

	class FontManager
	{
	private:
		static std::vector<Font*> m_Fonts;
	public:
		static void Add(Font* font);
		static Font* Get();
		static Font* Get(const std::string& name);
		static Font* Get(const std::string& name, unsigned int size);
		static void Clean();
	private:
		FontManager() { }
	};

} }