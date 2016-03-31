#pragma once

#include "Texture.h"

namespace sp { namespace graphics { namespace API {

	class SP_API Texture2D : public Texture
	{
	public:
		virtual void SetData(const void* pixels) = 0;
		virtual void SetData(uint color) = 0;

		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;
	public:
		static Texture2D* Create(uint width, uint height, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		static Texture2D* CreateFromFile(const String& filepath, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		static Texture2D* CreateFromFile(const String& filepath, TextureLoadOptions loadOptions);
		static Texture2D* CreateFromFile(const String& name, const String& filepath, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		static Texture2D* CreateFromFile(const String& name, const String& filepath, TextureLoadOptions loadOptions);
	};

} } }