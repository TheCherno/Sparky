#pragma once

#include "../graphics/VertexArray.h"

#include <graphics/shaders/Shader.h>

class Level;

class Entity
{
protected:
	VertexArray *model;
	sparky::graphics::Shader *shader;
	sparky::maths::vec3 position;
	Level *level;
public:
	Entity();
	virtual ~Entity();
	void init(Level *level);
	sparky::maths::vec3 get_position();
	virtual void update() = 0;
	virtual void render() = 0;
};
