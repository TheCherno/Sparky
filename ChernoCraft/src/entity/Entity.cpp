#include "Entity.h"
#include "../level/Level.h"

Entity::Entity()
{
}

Entity::~Entity()
{
    delete model;
}

void Entity::init(Level *level)
{
    this->level = level;
}

sparky::maths::vec3 Entity::get_position()
{
    return position;
}