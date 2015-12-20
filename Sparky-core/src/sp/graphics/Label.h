#pragma once

#include "sp/Common.h"
#include "Renderable2D.h"
#include "FontManager.h"

namespace sp { namespace graphics {

	class SP_API Label : public Renderable2D
	{
	public:
		Font* m_Font;
		String text;
		maths::vec3& position;
		float x, y;
	public:
		Label(const String& text, float x, float y, uint color);
		Label(const String& text, float x, float y, Font* font, uint color);
		Label(const String& text, float x, float y, const String& font, uint color);
		Label(const String& text, float x, float y, const String& font, uint size, uint color);
		void Submit(Renderer2D* renderer) const override;
		void ValidateFont(const String& name, int size = -1);
	};

} }