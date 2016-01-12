#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/maths/vec2.h"
#include "sp/graphics/Texture.h"

namespace ftgl {
	struct texture_atlas_t;
	struct texture_font_t;
}

namespace sp { namespace graphics {

	class SP_API Font
	{
	private:
		ftgl::texture_atlas_t* m_FTAtlas;
		ftgl::texture_font_t* m_FTFont;
		float m_Size;
		maths::vec2 m_Scale;
		String m_Name;
		String m_Filename;
		mutable Texture* m_Texture; // TODO: Replace with array
	public:
		Font(const String& name, const String& filename, float size);
		Font(const String& name, const byte* data, uint datasize, float size);

		void SetScale(float x, float y);
		uint GetID() const;

		inline void SetScale(const maths::vec2& scale) { m_Scale = scale; }
		inline const maths::vec2& GetScale() const { return m_Scale; }

		inline ftgl::texture_font_t* GetFTFont() const { return m_FTFont; }
		inline const String& GetName() const { return m_Name; }
		inline const String& GetFileName() const { return m_Filename; }
		inline float GetSize() const { return m_Size; }

		Texture* GetTexture() const;
	};

} }