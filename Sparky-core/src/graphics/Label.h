#pragma once

#include "Renderable2D.h"
#include "FontManager.h"

namespace sparky { namespace graphics {

	class Label : public Renderable2D
	{
	public:
		Font* m_Font;
		std::string text;
		maths::vec3& position;
		float x, y;
	public:
		Label(const std::string& text, float x, float y, uint color);
		Label(const std::string& text, float x, float y, Font* font, uint color);
		Label(const std::string& text, float x, float y, const std::string& font, uint color);
		Label(const std::string& text, float x, float y, const std::string& font, uint size, uint color);
		void Submit(Renderer2D* renderer) const override;
		void ValidateFont(const std::string& name, int size = -1);
	};

} }