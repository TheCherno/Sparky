#pragma once

#include <vector>
#include <sparky_types.h>

#include "Texture.h"

namespace sparky { namespace graphics {

	class TextureManager
	{
	private:
		static std::vector<Texture*> m_Textures;
	public:
		static void Add(Texture* texture);
		static Texture* Get(const std::string& name);
		static void Clean();
	private:
		TextureManager() { }
	};

} }