#include "sp/sp.h"
#include "Label.h"

namespace sp { namespace graphics {

	Label::Label(const String& text, float x, float y, uint color)
		: Sprite(), m_Text(text), m_Font(FontManager::Get("SourceSansPro"))
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, Font* font, uint color)
		: Sprite(), m_Text(text), m_Font(font)
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, const String& font, uint color)
		: Sprite(), m_Text(text), m_Font(FontManager::Get(font))
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		m_Color = color;

		ValidateFont(font);
	}

	Label::Label(const String& text, float x, float y, const String& font, uint size, uint color)
		: Sprite(), m_Text(text), m_Font(FontManager::Get(font, size))
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		m_Color = color;

		ValidateFont(font, size);
	}

	void Label::Submit(Renderer2D* renderer) const
	{
		renderer->DrawString(m_Text, GetPosition(), *m_Font, m_Color);
	}

	void Label::ValidateFont(const String& name, int32 size)
	{
		if (m_Font != nullptr)
			return;

		std::cout << "NULL FONT! Font=" << name;
		if (size > 0)
			std::cout << ", Size=" << size;
		std::cout << std::endl;

		m_Font = FontManager::Get("SourceSansPro");
	}

	void Label::SetText(const String& text)
	{
		m_Text = text;
		m_Bounds.size = m_Font->GetSize(text) / 2.0f;
	}

} }