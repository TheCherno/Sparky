#pragma once

#include "sp/Types.h"
#include "sp/String.h"

namespace sp { namespace graphics {

	enum class SP_API TextureWrap
	{
		REPEAT			= 0x2901, // GL_REPEAT
		CLAMP			= 0x2900, // GL_CLAMP
		MIRRORED_REPEAT = 0x8370, // GL_MIRRORED
		CLAMP_TO_EDGE	= 0x812F, // GL_CLAMP_TO_EDGE
		CLAMP_TO_BORDER	= 0x812D  // GL_CLAMP_TO_BORDER
	};

	enum class SP_API TextureFilter
	{
		LINEAR  = 0x2601, // GL_LINEAR
		NEAREST = 0x2600  // GL_NEAREST
	};

	class SP_API Texture
	{
	private:
		static TextureWrap s_WrapMode;
		static TextureFilter s_FilterMode;
	private:
		String m_Name, m_FileName;
		uint m_TID;
		uint m_Width, m_Height;
		unsigned int m_Bits;
	public:
		Texture(uint width, uint height, uint bits = 24);
		Texture(const String& name, const String& filename);
		Texture(uint glID);
		~Texture();
		void Bind() const;
		void Unbind() const;
		
		inline const String& GetName() const { return m_Name; }
		inline const uint GetID() const { return m_TID; }
		inline const uint GetWidth() const { return m_Width; }
		inline const uint GetHeight() const { return m_Height; }
	public:
		inline static void SetWrap(TextureWrap mode) { s_WrapMode = mode; }
		inline static void SetFilter(TextureFilter mode) { s_FilterMode = mode; }
	private:
		uint Load();
	};

} }