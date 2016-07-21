#pragma once

#include "sp/Common.h"
#include "Sprite.h"
#include "FontManager.h"

namespace sp { namespace graphics {

	class SP_API Label : public Sprite
	{
	public:
		enum class Alignment
		{
			NONE = 0, LEFT, CENTER, RIGHT
		};
	private:
		Font* m_Font;
		Alignment m_Alignment;
		maths::vec2 m_AlignmentOffset;
		String m_Text;
	public:
		Label(const String& text, float x, float y, uint color, Alignment alignment = Alignment::LEFT);
		Label(const String& text, float x, float y, Font* font, uint color, Alignment alignment = Alignment::LEFT);
		Label(const String& text, float x, float y, const String& font, uint color, Alignment alignment = Alignment::LEFT);
		Label(const String& text, float x, float y, const String& font, uint size, uint color, Alignment alignment = Alignment::LEFT);
		void Submit(Renderer2D* renderer) const override;
		void ValidateFont(const String& name, int32 size = -1);

		void SetAlignment(Alignment alignment);
		void SetText(const String& text);

		inline const Font& GetFont() const { return *m_Font; }
		inline Alignment GetAlignment() const { return m_Alignment; }
		inline const String& GetText() const { return m_Text; }
	private:
		void UpdateBounds();
	};

} }