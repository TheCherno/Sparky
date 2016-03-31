#include "sp/sp.h"
#include "Texture2D.h"

#include "sp/system/Memory.h"

#include "sp/platform/opengl/GLTexture2D.h"
#include "sp/platform/directx/DXTexture2D.h"

#include "sp/graphics/API/Context.h"

namespace sp { namespace graphics { namespace API {

	Texture2D* Texture2D::Create(uint width, uint height, TextureParameters parameters, TextureLoadOptions loadOptions)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTexture2D(width, height, parameters, loadOptions);
			case RenderAPI::DIRECT3D:	return spnew D3DTexture2D(width, height, parameters, loadOptions);
		}
		return nullptr;
	}

	Texture2D* Texture2D::CreateFromFile(const String& filepath, TextureParameters parameters, TextureLoadOptions loadOptions)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTexture2D(filepath, filepath, parameters, loadOptions);
			case RenderAPI::DIRECT3D:	return spnew D3DTexture2D(filepath, filepath, parameters, loadOptions);
		}
		return nullptr;
	}

	Texture2D* Texture2D::CreateFromFile(const String& filepath, TextureLoadOptions loadOptions)
	{
		switch (Context::GetRenderAPI())
		{
		case RenderAPI::OPENGL:		return spnew GLTexture2D(filepath, filepath, TextureParameters(), loadOptions);
		case RenderAPI::DIRECT3D:	return spnew D3DTexture2D(filepath, filepath, TextureParameters(), loadOptions);
		}
		return nullptr;
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const String& filepath, TextureParameters parameters, TextureLoadOptions loadOptions)
	{
		switch (Context::GetRenderAPI())
		{
			case RenderAPI::OPENGL:		return spnew GLTexture2D(name, filepath, parameters, loadOptions);
			case RenderAPI::DIRECT3D:	return spnew D3DTexture2D(name, filepath, parameters, loadOptions);
		}
		return nullptr;
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const String& filepath, TextureLoadOptions loadOptions)
	{
		switch (Context::GetRenderAPI())
		{
		case RenderAPI::OPENGL:		return spnew GLTexture2D(name, filepath, TextureParameters(), loadOptions);
		case RenderAPI::DIRECT3D:	return spnew D3DTexture2D(name, filepath, TextureParameters(), loadOptions);
		}
		return nullptr;
	}

} } }