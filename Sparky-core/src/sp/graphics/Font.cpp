#include "sp/sp.h"
#include "Font.h"

#include "sp/utils/Log.h"
#include <freetype-gl/freetype-gl.h>

namespace sp { namespace graphics {

	Font::Font(const String& name, const String& filename, float size)
		: m_Name(name), m_Filename(filename), m_Size(size), m_Scale(maths::vec2(1.0f, 1.0f)), m_Texture(nullptr)
	{
		using namespace API;

		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, size, filename.c_str());

		TextureParameters parameters = { TextureFormat::LUMINANCE_ALPHA, TextureFilter::LINEAR, TextureWrap::CLAMP_TO_EDGE };
		m_Texture = Texture2D::Create(512, 512, parameters);
		m_Texture->SetData(m_FTAtlas->data);

		SP_ASSERT(m_FTFont, "Failed to load font '", filename.c_str(), "'!");
	}

	Font::Font(const String& name, const byte* data, uint datasize, float size)
		: m_Name(name), m_Filename("NULL"), m_Size(size), m_Scale(maths::vec2(1.0f, 1.0f)), m_Texture(nullptr)
	{
		using namespace API;

		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_memory(m_FTAtlas, size, data, datasize);

		TextureParameters parameters = { TextureFormat::LUMINANCE_ALPHA, TextureFilter::LINEAR, TextureWrap::CLAMP_TO_EDGE };
		m_Texture = Texture2D::Create(512, 512, parameters);
		m_Texture->SetData(m_FTAtlas->data);

		SP_ASSERT(m_FTFont, "Failed to load font from data!");
	}

	void Font::SetScale(float x, float y)
	{
		m_Scale = maths::vec2(x, y);
	}

	API::Texture2D* Font::GetTexture() const
	{
		UpdateAtlas();
		return m_Texture;
	}

	void Font::UpdateAtlas() const
	{
		if (m_FTAtlas->dirty)
		{
			m_Texture->SetData(m_FTAtlas->data);
			m_FTAtlas->dirty = false;
		}
	}

	maths::vec2 Font::GetOffsets(const String& text) const
	{
		using namespace ftgl;

		if (text.empty())
			return maths::vec2(0.0f, 0.0f);

		texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[0]);
		SP_ASSERT(glyph);

		float yo = 0.0f;
		const maths::vec2& scale = m_Scale;
		for (int i = 0; i < text.size(); i++)
		{
			texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
			SP_ASSERT(glyph);
			float height = glyph->height / scale.y;
			float offset = glyph->offset_y / scale.y - height;
			if (offset < yo)
				yo = offset;
		}

		return maths::vec2(glyph->offset_x / scale.x, yo);
	}

	float Font::GetWidth(const String& text) const
	{
		using namespace ftgl;

		float width = 0.0f;
		const maths::vec2& scale = m_Scale;
		for (int i = 0; i < text.size(); i++)
		{
			texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
			SP_ASSERT(glyph);
			if (i > 0)
			{
				float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
				width += kerning / scale.x;
			}
			width += glyph->advance_x / scale.x;
		}
		return width;
	}

	float Font::GetHeight(const String& text) const
	{
		using namespace ftgl;

		const maths::vec2& scale = m_Scale;
		float min = 0.0f;
		float max = 0.0f;
		for (int i = 0; i < text.size(); i++)
		{
			texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
			SP_ASSERT(glyph);
			float height = glyph->height / scale.y;
			float offset = glyph->offset_y / scale.y - height;
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}
		return abs(min) + abs(max);
	}

	maths::vec2 Font::GetSize(const String& text) const
	{
		return maths::vec2(GetWidth(text), GetHeight(text));
	}

	maths::Rectangle Font::GetBounds(const String& text) const
	{
		return maths::Rectangle(0.0f, 0.0f, GetWidth(text), GetHeight(text));
	}

} }