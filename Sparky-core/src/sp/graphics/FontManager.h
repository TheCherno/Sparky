#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "Font.h"

namespace sp { namespace graphics {

	class SP_API FontManager
	{
	private:
		static std::vector<Font*> s_Fonts;
		static maths::vec2 s_Scale;
	public:
		static void SetScale(const maths::vec2& scale);

		static void Add(Font* font);
		static Font* Get();
		static Font* Get(const String& name);
		static Font* Get(uint size);
		static Font* Get(const String& name, uint size);
		static void Clean();
	private:
		FontManager() { }
	};

} }