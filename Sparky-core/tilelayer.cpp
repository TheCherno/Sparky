#include "tilelayer.h"

#include "../maths/tvec2.h"

TileLayer::TileLayer(sparky::graphics::Shader* shader)
	: Layer(new sparky::graphics::BatchRenderer2D(maths::tvec2<uint>(0,0)), shader, sparky::maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
{
}

TileLayer::~TileLayer()
{
}
