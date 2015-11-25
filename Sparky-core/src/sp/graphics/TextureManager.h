#pragma once

#include <vector>
#include <sp/Types.h>

#include "Texture.h"

namespace sp { namespace graphics {

	class TextureManager
	{
	private:
		static std::vector<Texture*> m_Textures;
	public:
		static void Add(Texture* texture);
		static Texture* Get(const String& name);
		static void Clean();
	private:
		TextureManager() { }
	};

} }