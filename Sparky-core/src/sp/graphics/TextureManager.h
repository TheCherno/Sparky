#pragma once

#include "sp/sp.h"
#include "sp/Types.h"
#include "sp/graphics/Texture.h"

namespace sp { namespace graphics {

	class SP_API TextureManager
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