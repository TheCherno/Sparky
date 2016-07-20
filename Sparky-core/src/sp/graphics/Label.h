#pragma once

#include "sp/Common.h"
#include "Sprite.h"
#include "FontManager.h"

namespace sp { namespace graphics {

	class SP_API Label : public Sprite
	{
	private:
		Font* m_Font;
		String m_Text;
	public:
		Label(const String& text, float x, float y, uint color);
		Label(const String& text, float x, float y, Font* font, uint color);
		Label(const String& text, float x, float y, const String& font, uint color);
		Label(const String& text, float x, float y, const String& font, uint size, uint color);
		void Submit(Renderer2D* renderer) const override;
		void ValidateFont(const String& name, int32 size = -1);

		void SetText(const String& text);

		inline const Font& GetFont() const { return *m_Font; }
		inline const String& GetText() const { return m_Text; }
	};

} }