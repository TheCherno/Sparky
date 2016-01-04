#include "sp/sp.h"
#include "Font.h"

#include "sp/utils/Log.h"
#include <freetype-gl/freetype-gl.h>

namespace sp { namespace graphics {

	Font::Font(const String& name, const String& filename, float size)
		: m_Name(name), m_Filename(filename), m_Size(size), m_Scale(maths::vec2(1.0f, 1.0f)), m_Texture(nullptr)
	{
		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, size, filename.c_str());
		SP_ASSERT(m_FTFont, "Failed to load font '", filename.c_str(), "'!");
	}

	Font::Font(const String& name, const byte* data, uint datasize, float size)
		: m_Name(name), m_Filename("NULL"), m_Size(size), m_Scale(maths::vec2(1.0f, 1.0f)), m_Texture(nullptr)
	{
		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_memory(m_FTAtlas, size, data, datasize);
		SP_ASSERT(m_FTFont, "Failed to load font from data!");
	}

	void Font::SetScale(float x, float y)
	{
		m_Scale = maths::vec2(x, y);
	}

	uint Font::GetID() const
	{
		return m_FTAtlas->id;
	}

	Texture* Font::GetTexture() const
	{
		if (m_Texture != nullptr || !m_FTAtlas->id)
			return m_Texture;

		m_Texture = new Texture(m_FTAtlas->id);
	}

} }