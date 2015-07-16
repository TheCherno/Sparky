#include "Label.h"

namespace sparky { namespace graphics {

	Label::Label(const std::string& text, float x, float y, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(FontManager::Get("SourceSansPro"))
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;
	}

	Label::Label(const std::string& text, float x, float y, Font* font, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(font)
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;
	}

	Label::Label(const std::string& text, float x, float y, const std::string& font, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(FontManager::Get(font))
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;

		ValidateFont(font);
	}

	Label::Label(const std::string& text, float x, float y, const std::string& font, uint size, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(FontManager::Get(font, size))
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;

		ValidateFont(font, size);
	}

	void Label::Submit(Renderer2D* renderer) const
	{
		renderer->DrawString(text, position, *m_Font, m_Color);
	}

	void Label::ValidateFont(const std::string& name, int size)
	{
		if (m_Font != nullptr)
			return;

		std::cout << "NULL FONT! Font=" << name;
		if (size > 0)
			std::cout << ", Size=" << size;
		std::cout << std::endl;

		m_Font = FontManager::Get("SourceSansPro");
	}


} }