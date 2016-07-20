#include "sp/sp.h"
#include "Sprite.h"

namespace sp { namespace graphics {

	Sprite::Sprite()
		: Renderable2D()
	{
	}

	Sprite::Sprite(API::Texture2D* texture)
		: Renderable2D(maths::vec3(0.0f, 0.0f, 0.0f), maths::vec2((float)texture->GetWidth(), (float)texture->GetHeight()), 0xffffffff)
	{
		m_Texture = texture;
	}

	Sprite::Sprite(float x, float y, API::Texture2D* texture)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2((float)texture->GetWidth(), (float)texture->GetHeight()), 0xffffffff)
	{
		m_Texture = texture;
	}

	Sprite::Sprite(float x, float y, float width, float height, uint color)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), color)
	{
	}

	Sprite::Sprite(float x, float y, float width, float height, const maths::vec4& color)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), 0xffffffff)
	{
		SetColor(color);
	}

	Sprite::Sprite(float x, float y, float width, float height, API::Texture* texture)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), 0xffffffff)
	{
		m_Texture = texture;
	}

	void Sprite::SetUV(const std::vector<maths::vec2>& uv)
	{
		m_UVs = uv;
	}

} }