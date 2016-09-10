#include "sp/sp.h"
#include "TextureCube.h"

#include "sp/system/Memory.h"

#include "sp/platform/opengl/GLTextureCube.h"
#ifdef SP_PLATFORM_WIN32
#include "sp/platform/directx/DXTextureCube.h"
#endif

#include "sp/graphics/API/Context.h"

namespace sp { namespace graphics { namespace API {

	TextureCube* TextureCube::CreateFromFile(const String& filepath)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTextureCube(filepath, filepath);
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return spnew D3DTextureCube(filepath, filepath);
#endif
		}
		return nullptr;
	}

	TextureCube* TextureCube::CreateFromFiles(const String* files)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTextureCube(files[0], files);
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return spnew D3DTextureCube(files[0], files);
#endif
		}
		return nullptr;
	}

	TextureCube* TextureCube::CreateFromVCross(const String* files, int32 mips)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTextureCube(files[0], files, mips, InputFormat::VERTICAL_CROSS);
#ifdef SP_PLATFORM_WIN32
			case RenderAPI::DIRECT3D:	return spnew D3DTextureCube(files[0], files, mips, InputFormat::VERTICAL_CROSS);
#endif
		}
		return nullptr;
	}

} } }