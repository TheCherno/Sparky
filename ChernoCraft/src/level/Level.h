#pragma once

#include <vector>
#include <maths/maths.h>
#include <graphics/shaders/Shader.h>

#include "block/Block.h"
#include "../graphics/VertexArray.h"
#include "../entity/Entity.h"
#include "../entity/Player.h"

class Level {
public:
    static unsigned short NULL_BLOCK;
    static sparky::maths::vec3 NULL_COORD;
private:
    VertexArray* floor;
    unsigned short*** blocks;
	sparky::maths::mat4 floor_pos, floor_rot;
    std::vector<Entity*> entities;
    
    const int WIDTH = 16;
    const int DEPTH = 16;
    const int HEIGHT = 16;
	sparky::maths::vec3 selected_block;
public:
    Level();
    ~Level();
    void add(Entity *entity);
    void update();
	void select_block(const sparky::maths::vec3& block);
    unsigned short& get_intersecting_block(Entity *entity);
	unsigned short& get_block(const sparky::maths::vec3& position);
	unsigned short& raycast_collision(const sparky::maths::vec3& position, const sparky::maths::vec3& rotation);
	unsigned short& pick_block(const sparky::maths::vec3& position, sparky::maths::vec3& rotation);
    unsigned short& raycast_block_id(const sparky::maths::vec3& position, sparky::maths::vec3& rotation);
    unsigned short& raycast_block_id(const sparky::maths::vec3& position, sparky::maths::vec3& rotation, float distance);
	unsigned short& raycast_pre_block_id(const sparky::maths::vec3& position, sparky::maths::vec3& rotation);
	sparky::maths::vec3 raycast_block(const sparky::maths::vec3& position, sparky::maths::vec3& rotation);
    void render();
};
