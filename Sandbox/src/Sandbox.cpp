#include <Sparky.h>

#include "TestLayer.h"
#include "Test3D.h"

using namespace sp;
using namespace graphics;
using namespace maths;

#define WIDTH	1280
#define HEIGHT	720

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
		PushLayer(new Test3D());
		//PushOverlay(new TestLayer());
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}