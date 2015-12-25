#pragma once

#include <app/Application.h>
#include "level/level.h"

class Game : public sparky::Application
{
public:
	static sparky::graphics::Window* WindowHandle;
private:
	Level* m_Level;
public:
	Game();
	~Game();

	void Init() override;
	void Update() override;
	void Render() override;

};