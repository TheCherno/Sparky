#include "Block.h"

#include "AirBlock.h"
#include "DirtBlock.h"
#include "StoneBlock.h"

#include <time.h>
#include "../../Shaders.h"

using namespace sparky;
using namespace maths;

const float Block::SIZE = 8.0f;
Block* Block::air = (Block*) NULL;
Block* Block::stone = (Block*) NULL;
Block* Block::dirt = (Block*) NULL;
VertexArray* Block::vao = (VertexArray*) NULL;

Block::Block(unsigned short ID)
{
    this->ID = ID;
    if (vao == (VertexArray*) NULL) {
        _SPARKY_INFO("Initializing block... ");
        init();
		_SPARKY_INFO("done.\n");
    }
    srand((unsigned int) time(NULL));
    color = vec3(0.3, 0.6, 0.2);
    shader = Shaders::BLOCK;
	SPARKY_INFO("Creating block...");
}

void Block::CREATE_ALL()
{
    air = new AirBlock(1);
    stone = new StoneBlock(2);
    dirt = new DirtBlock(3);
}

void Block::DESTROY()
{
    delete air;
    delete stone;
    delete dirt;
    delete vao;
}

void Block::init()
{
/*    
    GLfloat vertices[8 * 3] = {
        -SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f,
         SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f,
         SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f,
        -SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f,
        -SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f,
         SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f,
         SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f,
        -SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f
    };
    
    GLuint indices[6 * 6] = {
        0, 1, 2, 2, 3, 0,
        3, 2, 6, 6, 7, 3,
        7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4,
        0, 5, 1, 5, 0, 4, // Bottom
        1, 5, 6, 6, 2, 1
    };
*/
    
    GLfloat vertices[6 * 4 * 3] = {
        // Front
        -SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f, // 0
         SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f, // 1
         SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f, // 2
        -SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f, // 3
        
        // Back
        -SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f, // 4
        -SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f, // 5
         SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f, // 6
         SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f, // 7

        // Top
        -SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f, // 8
        -SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f, // 11
         SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f, // 10
         SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f, // 9

        // Bottom
        -SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f, // 12
         SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f, // 13
         SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f, // 14
        -SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f, // 15

        // Left
        -SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f, // 16
        -SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f, // 17
        -SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f, // 18
        -SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f, // 19

        // Right
         SIZE / 2.0f, -SIZE / 2.0f, -SIZE / 2.0f, // 20
         SIZE / 2.0f,  SIZE / 2.0f, -SIZE / 2.0f,  // 23
         SIZE / 2.0f,  SIZE / 2.0f,  SIZE / 2.0f, // 22
         SIZE / 2.0f, -SIZE / 2.0f,  SIZE / 2.0f // 21
    };
    
    GLuint indices[6 * 6] = {
         0,  1,  2,  2,  3,  0, // Front
         4,  5,  6,  6,  7,  4, // Back
         8,  9, 10, 10, 11,  8, // Top
        12, 13, 14, 14, 15, 12, // Bottom
        16, 17, 18, 18, 19, 16, // Left
        20, 21, 22, 22, 23, 20  // Right
    };

    
    GLfloat normals[6 * 4 * 3] = {
        // Front
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,

        // Back
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,

        // Top
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
        
        // Bottom
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
 
        // Left
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

        // Right
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f
    };
    
    GLfloat tcs[24 * 6 * 2] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };
    
    vao = new VertexArray(vertices, indices, normals, tcs, 6 * 4 * 3, 6 * 6);
}

void Block::update()
{
}

void Block::render(const vec3& position)
{
    shader->Bind();
    texture->enable();
	mat4 pos = mat4::Translate(position);
    shader->SetUniformMat4("ml_matrix", pos);
    vao->render();
    texture->disable();
    shader->Unbind();
}
