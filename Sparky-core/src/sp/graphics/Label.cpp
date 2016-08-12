#include "sp/sp.h"
#include "Label.h"

namespace sp { namespace graphics {

	Label::Label(const String& text, float x, float y, uint color, Alignment alignment)
		: Sprite(), m_Font(FontManager::Get("SourceSansPro"))
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		SetText(text);
		SetAlignment(alignment);
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, Font* font, uint color, Alignment alignment)
		: Sprite(), m_Font(font)
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		SetText(text);
		SetAlignment(alignment);
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, const String& font, uint color, Alignment alignment)
		: Sprite(), m_Font(FontManager::Get(font))
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		SetText(text);
		SetAlignment(alignment);
		m_Color = color;

		ValidateFont(font);
	}

	Label::Label(const String& text, float x, float y, const String& font, uint size, uint color, Alignment alignment)
		: Sprite(), m_Font(FontManager::Get(font, size))
	{
		SetPosition(maths::vec3(x, y, 0.0f));
		SetText(text);
		SetAlignment(alignment);
		m_Color = color;

		ValidateFont(font, size);
	}

	void Label::Submit(Renderer2D* renderer) const
	{
		renderer->DrawString(m_Text, m_Bounds.GetMinimumBound() + m_AlignmentOffset, *m_Font, m_Color);
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

	void Label::SetAlignment(Alignment alignment)
	{
		m_Alignment = alignment;
		UpdateBounds();
	}

	void Label::SetText(const String& text)
	{
		m_Text = text;
		UpdateBounds();
	}

	void Label::UpdateBounds()
	{
		using namespace maths;

		vec2 size = m_Font->GetSize(m_Text) * 0.5f;
		m_Bounds.size = size;
		switch (m_Alignment)
		{
		case Alignment::LEFT:
			m_AlignmentOffset.x = size.x;
			break;
		case Alignment::CENTER:
			m_AlignmentOffset.x = 0.0f;
			break;
		case Alignment::RIGHT:
			m_AlignmentOffset.x = -size.x;
			break;
		}
	}

} }