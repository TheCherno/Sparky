#include "tilelayer.h"

TileLayer::TileLayer(sparky::graphics::Shader* shader)
	: Layer(new sparky::graphics::BatchRenderer2D(), shader, sparky::maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
{
}

TileLayer::~TileLayer()
{
}
