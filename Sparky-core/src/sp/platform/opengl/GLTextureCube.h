#pragma once

#include "sp/graphics/API/TextureCube.h"
#include "GLCommon.h"

namespace sp { namespace graphics { namespace API {

	class GLTextureCube : public TextureCube
	{
	private:
		uint m_Handle;
		uint m_Width, m_Height;
		String m_Name;
		String* m_Files;
		uint m_Bits;
		InputFormat m_Format;
		TextureParameters m_Parameters;
		TextureLoadOptions m_LoadOptions;
	public:
		GLTextureCube(const String& name, const String& filepath);
		GLTextureCube(const String& name, const String* files);
		GLTextureCube(const String& name, const String* files, uint mips, InputFormat format);
		~GLTextureCube();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilepath() const override { return m_Files[0]; }
	private:
		uint LoadFromSingleFile();
		uint LoadFromMultipleFiles();
		uint LoadFromVCross(uint mips);
	};


} } }
