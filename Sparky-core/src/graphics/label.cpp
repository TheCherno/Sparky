#include "label.h"

namespace sparky { namespace graphics {

	Label::Label(std::string text, float x, float y, unsigned int color)
		: Renderable2D(), text(text), position(m_Position)
	{
		m_Position = maths::vec3(x, y, 0.0f);
		m_Color = color;
	}

	void Label::submit(Renderer2D* renderer) const
	{
		renderer->drawString(text, position, m_Color);
	}

} }