#include "sprite.h"

namespace sparky { namespace graphics {

	Sprite::Sprite(Texture* texture)
		: Renderable2D(maths::vec3(0, 0, 0), maths::vec2(texture->getWidth(), texture->getHeight()), 0xffffffff), position(m_Position), size(m_Size)
	{
		m_Texture = texture;
	}

	Sprite::Sprite(float x, float y, Texture* texture)
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(texture->getWidth(), texture->getHeight()), 0xffffffff), position(m_Position), size(m_Size)
	{
		m_Texture = texture;
	}

	Sprite::Sprite(float x, float y, float width, float height, unsigned int color)
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), color), position(m_Position), size(m_Size)
	{

	}

	Sprite::Sprite(float x, float y, float width, float height, maths::vec4 color)
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), 0), position(m_Position), size(m_Size)
	{
		setColor(color);
	}

	Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
		: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), 0xffffffff), position(m_Position), size(m_Size)
	{
		m_Texture = texture;
	}

	void Sprite::setUV(std::vector<maths::vec2> uv)
	{
		m_UV = uv;
	}

} }