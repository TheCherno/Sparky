#pragma once

#include "Texture.h"

namespace sp { namespace graphics { namespace API {

	class SP_API TextureCube : public Texture
	{
	protected:
		enum class InputFormat
		{
			VERTICAL_CROSS,
			HORIZONTAL_CROSS
		};
	public:
		static TextureCube* CreateFromFile(const String& filepath);
		static TextureCube* CreateFromFiles(const String* files);
		static TextureCube* CreateFromVCross(const String* files, int32 mips);
	};

} } }