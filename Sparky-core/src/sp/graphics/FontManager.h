#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "Font.h"

namespace sp { namespace graphics {

	class SP_API FontManager
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