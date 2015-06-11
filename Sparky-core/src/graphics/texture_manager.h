#pragma once

#include <vector>
#include "texture.h"

namespace sparky { namespace graphics {

	class TextureManager
	{
	private:
		static std::vector<Texture*> m_Textures;
	public:
		static void add(Texture* texture);
		static Texture* get(const std::string& name);
		static void clean();
	private:
		TextureManager() { }
	};

} }