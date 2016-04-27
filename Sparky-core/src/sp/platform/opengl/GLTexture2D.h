#pragma once

#include "sp/graphics/API/Texture2D.h"
#include "GLCommon.h"

namespace sp { namespace graphics { namespace API {

	class GLTexture2D : public Texture2D
	{
	private:
		String m_Name;
		String m_FileName;
		uint m_Handle;
		uint m_Width, m_Height;
		TextureParameters m_Parameters;
		TextureLoadOptions m_LoadOptions;
	public:
		GLTexture2D(uint width, uint height, TextureParameters parameters = TextureParameters(), bool renderTarget = false);
		GLTexture2D(uint width, uint height, uint color, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		GLTexture2D(const String& name, const String& filename, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());

		~GLTexture2D();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		virtual void SetData(const void* pixels) override;
		virtual void SetData(uint color) override;

		uint GetHandle() const { return m_Handle; }

		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight() const { return m_Height; }

		inline const String& GetName() const { return m_Name; }
		inline const String& GetFilepath() const { return m_FileName; }
	private:
		uint Load();
	public:
		static uint SPTextureFormatToGL(TextureFormat format);
		static uint SPTextureWrapToGL(TextureWrap wrap);
	};


} } }