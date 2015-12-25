#pragma once

#include "Entity.h"

#include <graphics/Window.h>

class Player : public Entity
{
private:
    sparky::maths::vec3 rotation;
    float mouse_sensitivity;
    float dy;
    const float JUMP_HEIGHT;
    bool jumping;
    bool light;
    float speed;
    VertexArray* target;
	sparky::graphics::Window& m_Window;
	bool m_Wireframe;
public:
	Player(const sparky::maths::vec3& spawn);
    void update();
    void render();
};