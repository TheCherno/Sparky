#include <Sparky.h>

#include "TestLayer.h"
#include "Test3D.h"

using namespace sp;
using namespace graphics;
using namespace maths;

#define WIDTH 960
#define HEIGHT 540

class Game : public Application
{
public:
	Game()
		: Application("Sandbox", WIDTH, HEIGHT)
	{
	}

	~Game()
	{
	}

	void Init() override
	{
		Application::Init();
		// PushLayer(new TestLayer());
		PushLayer(new Test3D());
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}