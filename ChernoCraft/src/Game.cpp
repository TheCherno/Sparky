#include "game.h"
#include "Shaders.h"

using namespace sparky;

sparky::graphics::Window* Game::WindowHandle = nullptr;

Game::Game()
	: Application("ChernoCraft", 960, 540)
{
}

void Game::Init()
{
	Application::Init();

	WindowHandle = window;
	Shaders::Init();
	Texture::LOAD();
	m_Level = new Level();
}

Game::~Game()
{
	delete m_Level;
}

void Game::Update()
{
	m_Level->update();
}

void Game::Render()
{
	m_Level->render();
}