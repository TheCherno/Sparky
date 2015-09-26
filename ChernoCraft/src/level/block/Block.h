#pragma once

#include "../../graphics/VertexArray.h"
#include "../../graphics/Texture.h"
#include <maths/maths.h>
#include <graphics/shaders/Shader.h>

class Block
{
public:
    static Block *air, *dirt, *stone;
    const static float SIZE;
protected:
    unsigned short ID;
    static VertexArray* vao;
    sparky::graphics::Shader* shader;
    sparky::maths::vec3 color;
    Texture* texture;
public:
    Block(unsigned short ID);
    static void CREATE_ALL();
    static void DESTROY();
    virtual void update();
    virtual void render(const sparky::maths::vec3& position);
private:
    void init();
};