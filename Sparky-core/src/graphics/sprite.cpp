#include "sprite.h"

namespace sparky { namespace graphics {

	Sprite::Sprite(float x, float y, float width, float height, const maths::vec4& color)
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), color)
	{

	}

	Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), maths::vec4(1, 1, 1, 1))
	{
		m_Texture = texture;
	}

} }