#pragma once

#include <string>
#include "../maths/vec2.h"
#include "../../ext/freetype-gl/freetype-gl.h"

namespace sparky { namespace graphics {

	class Font
	{
	private:
		ftgl::texture_atlas_t* m_FTAtlas;
		ftgl::texture_font_t* m_FTFont;
		float m_Size;
		maths::vec2 m_Scale;
		std::string m_Name;
		std::string m_Filename;
	public:
		Font(const std::string& name, const std::string& filename, float size);
		Font(const std::string& name, const byte* data, unsigned int datasize, float size);

		void SetScale(float x, float y);

		inline ftgl::texture_font_t* GetFTFont() const { return m_FTFont; }

		inline const unsigned int GetID() const { return m_FTAtlas->id; }
		inline const maths::vec2& GetScale() const { return m_Scale; }
		inline const std::string& GetName() const { return m_Name; }
		inline const std::string& GetFileName() const { return m_Filename; }
		inline const float GetSize() const { return m_Size; }
	};

} }