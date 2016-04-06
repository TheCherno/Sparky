#pragma once

#include "sp/sp.h"
#include "sp/Types.h"
#include "sp/graphics/API/Texture.h"

namespace sp { namespace graphics {

	class SP_API TextureManager
	{
	private:
		static std::vector<API::Texture*> m_Textures;
	public:
		static API::Texture* Add(API::Texture* texture);
		static API::Texture* Get(const String& name);
		static void Clean();
	private:
		TextureManager() { }
	};

} }