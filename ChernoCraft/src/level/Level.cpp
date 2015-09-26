#include "Level.h"
#include <time.h>

#include "../Shaders.h"

using namespace sparky;
using namespace maths;

unsigned short Level::NULL_BLOCK = 0;
vec3 Level::NULL_COORD = vec3(-1.0, -1.0, -1.0);

Level::Level()
{
    add(new Player(vec3(-45.0f, 48.0f, -45.0f)));
    blocks = new unsigned short**[WIDTH];
    for (unsigned int x = 0; x < WIDTH; x++)
	{
        blocks[x] = new unsigned short*[DEPTH];
        for (unsigned int y = 0; y < DEPTH; y++)
		{
            blocks[x][y] = new unsigned short[HEIGHT];
        }
    }
    
    srand((unsigned int) time(NULL));
    for (unsigned int z = 0; z < HEIGHT; z++)
	{
        for (unsigned int y = 0; y < DEPTH; y++)
		{
            for (unsigned int x = 0; x < WIDTH; x++)
			{
                if (z < 1)
                    blocks[x][y][z] = 2;
                else if (z < 4 && rand() % 3 == 0)
                    blocks[x][y][z] = 3;
                else if (z < 4)
                    blocks[x][y][z] = 2;
                else if (z < 5)
                    blocks[x][y][z] = 3;
                else if (z < HEIGHT && rand() % 30 == 0)
                    blocks[x][y][z] = 2;
                else
                    blocks[x][y][z] = 1;
            }
        }
    }
    Block::CREATE_ALL();
}

Level::~Level()
{
    for (unsigned int y = 0; y < DEPTH; y++)
        delete[] blocks[y];

    delete[] blocks;
    Block::DESTROY();
}

void Level::add(Entity *entity)
{
    entity->init(this);
    entities.push_back(entity);
}

void Level::update()
{
    for (int i = 0; i < entities.size(); i++)
        entities[i]->update();
}

void Level::select_block(const vec3& block)
{
    this->selected_block = block;
}

unsigned short& Level::get_intersecting_block(Entity *entity)
{
	vec3 position = vec3(entity->get_position().x / Block::SIZE, entity->get_position().y / Block::SIZE, entity->get_position().z / Block::SIZE);
    if (position.x < 0 || position.y < 0 || position.z < 0) return NULL_BLOCK;
    if (position.x >= WIDTH || position.y >= HEIGHT || position.z >= DEPTH) return NULL_BLOCK;
    return blocks[(short) position.x][(short) position.z][(short) position.y];
}

unsigned short& Level::get_block(const vec3& position)
{
	vec3 pos = vec3(position.x / Block::SIZE, position.y / Block::SIZE, position.z / Block::SIZE);

	if (pos.x < 0 || pos.y < 0 || pos.z < 0)
		return NULL_BLOCK;

	if (pos.x >= WIDTH || pos.y >= HEIGHT || pos.z >= DEPTH)
		return NULL_BLOCK;

	return blocks[(short)pos.x][(short)pos.z][(short)pos.y];
}

unsigned short& Level::pick_block(const vec3& position, vec3& rotation)
{
    const float MAX_DISTANCE = 32.0f;
	vec3 rot = vec3(cos(toRadians(rotation.y - 90.0f)), -tan(toRadians(rotation.x)), sin(toRadians(rotation.y - 90.0f)));
    return raycast_block_id(position, rot, MAX_DISTANCE);
}

vec3 Level::raycast_block(const vec3& position, vec3 &rotation)
{
	vec3 pos = vec3(-position.x, -position.y, -position.z);
	vec3 ray_vector = vec3(cos(toRadians(rotation.y - 90.0f)), -tan(toRadians(rotation.x)), sin(toRadians(rotation.y - 90.0f)));
    const float MAX_DISTANCE = 32.0f;
    const float ITERATION = 0.5f;
    float distance = 0.0f;
//    vec4 ray_eye = inverse(Shader::pr_matrix) * vec4(ray_vector.x, ray_vector.y, ray_vector.z, 1.0f);
    //ray_vector = vec3(ray_eye.x, ray_eye.y, ray_eye.z);
    while (distance < MAX_DISTANCE)
	{
		pos += ray_vector * ITERATION;
        unsigned short &result = get_block(pos);

		if (result != 1 && result != NULL_BLOCK)
			return vec3((short)(pos.x / Block::SIZE), (short)(pos.z / Block::SIZE), (short)(pos.y / Block::SIZE));

        distance += ITERATION;
    }
    return NULL_COORD;
}

unsigned short& Level::raycast_block_id(const vec3& position, vec3 &rotation)
{
    const float MAX_DISTANCE = 32.0f;
    return raycast_block_id(position, rotation, MAX_DISTANCE);
}

unsigned short& Level::raycast_block_id(const vec3& position, vec3 &rotation, float distance)
{
	vec3 pos = vec3(-position.x, -position.y, -position.z);
    const float ITERATION = 0.5f;
    float length = 0.0f; // Ray length
    while (length < distance)
	{
        pos += rotation * ITERATION;
        unsigned short &result = get_block(pos);

        if (result != 1 && result != NULL_BLOCK)
			return result;

        length += ITERATION;
    }
    return NULL_BLOCK;
}

unsigned short& Level::raycast_collision(const vec3& position, const vec3& rotation)
{
	vec3 pos = vec3(-position.x, -position.y, -position.z);
	const float MAX_DISTANCE = 2.0f;
	const float ITERATION = 0.5f;
	float distance = 0.0f;
	while (distance < MAX_DISTANCE)
	{
		pos += rotation * ITERATION;
		unsigned short &result = get_block(pos);

		if (result != 1 && result != NULL_BLOCK)
			return result;

		distance += ITERATION;
	}
	return NULL_BLOCK;
}

unsigned short& Level::raycast_pre_block_id(const vec3& position, vec3& rotation)
{
	vec3 pos = vec3(-position.x, -position.y, -position.z);
    vec3 ray_vector = vec3(cos(toRadians(rotation.y - 90.0f)), -tan(toRadians(rotation.x)), sin(toRadians(rotation.y - 90.0f)));
    const float MAX_DISTANCE = 32.0f;
    const float ITERATION = 0.5f;
    float distance = 0.0f;
    while (distance < MAX_DISTANCE)
	{
        pos += ray_vector * ITERATION;
        unsigned short &result = get_block(pos);
		vec3 a = pos - ray_vector;

		if (result != 1 && result != NULL_BLOCK)
			return get_block(vec3(a.x * ITERATION, a.y * ITERATION, a.z * ITERATION));

        distance += ITERATION;
    }
    return NULL_BLOCK;
}

void Level::render()
{
    Shaders::BLOCK->Bind();
    for (int z = 0; z < HEIGHT; z++)
	{
        for (int y = 0; y < DEPTH; y++)
		{
            for (int x = 0; x < WIDTH; x++)
			{
                Block *block = Block::air;
                switch (blocks[x][y][z])
				{
                    case 2:
                        block = Block::stone;
                        break;
                    case 3:
                        block = Block::dirt;
                        break;
                }
				Shaders::BLOCK->Bind();
//                std::cout << z << ": " << selected_block.z << std::endl;

                if (vec3(x, y, z) == selected_block)
					Shaders::BLOCK->SetUniform1f("selected", 1.0f);
                else
					Shaders::BLOCK->SetUniform1f("selected", 0.0f);

				Shaders::BLOCK->Unbind();
                block->render(vec3(x * Block::SIZE + 2.0f, z * Block::SIZE + 2.0f, y * Block::SIZE + 2.0f));
            }
        }
    }

    for (int i = 0; i < entities.size(); i++)
        entities[i]->render();

	Shaders::BLOCK->Unbind();
}