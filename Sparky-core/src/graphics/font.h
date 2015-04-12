#pragma once

#include <string>
#include "../../ext/freetype-gl/freetype-gl.h"

namespace sparky { namespace graphics {

	class Font
	{
	private:
		ftgl::texture_atlas_t* m_FTAtlas;
		ftgl::texture_font_t* m_FTFont;
		unsigned int m_Size;

		std::string m_Name;
		std::string m_Filename;
	public:
		Font(std::string name, std::string filename, int size);

		inline ftgl::texture_font_t* getFTFont() const { return m_FTFont; }

		inline const unsigned int getID() const { return m_FTAtlas->id; }
		inline const std::string& getName() const { return m_Name; }
		inline const std::string& getFileName() const { return m_Filename; }
		inline const int getSize() const { return m_Size; }
	};

} }