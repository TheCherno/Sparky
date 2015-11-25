#include <sp/sparky.h>
#include <sp/app/Application.h>

#include "TestLayer.h"

using namespace sp;
using namespace graphics;
using namespace maths;

#define WIDTH 960
#define HEIGHT 540

class Game : public Application
{
public:
	Game()
		: Application("Test Game", WIDTH, HEIGHT)
	{
	}

	~Game()
	{
	}

	void Init() override
	{
		Application::Init();
		PushLayer(new TestLayer());
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}