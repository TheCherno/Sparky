#include "sp/sp.h"
#include "Label.h"

namespace sp { namespace graphics {

	Label::Label(const String& text, float x, float y, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(FontManager::Get("SourceSansPro"))
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, Font* font, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(font)
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, const String& font, uint color)
		: Renderable2D(), text(text), position(m_Position), m_Font(FontManager::Get(font))
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;

		ValidateFont(font);
	}

	Label::Label(const String& text, float x, float y, const String& font, uint size, uint color)
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

	void Label::ValidateFont(const String& name, int size)
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