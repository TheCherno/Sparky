#pragma once

#include <graphics/shaders/Shader.h>

class Shaders
{
public:
	static sparky::graphics::Shader* BLOCK;
	static sparky::graphics::Shader* HUD;
public:
	static void Init();
};