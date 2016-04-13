#include <Sparky.h>

#include "Test2D.h"
#include "Test3D.h"
#include "DeferredTest.h"
#include "SponzaTest.h"

using namespace sp;
using namespace graphics;
using namespace maths;

#define WIDTH	1280
#define HEIGHT	720

class Game : public Application
{
public:
	Game()
		: Application("Sandbox", WIDTH, HEIGHT, sp::graphics::API::RenderAPI::DIRECT3D)
	{
	}

	~Game()
	{
	}

	void Init() override
	{
		Application::Init();
		VFS::Get()->Mount("models", "res/models");
		VFS::Get()->Mount("pbr", "res/pbr");
		VFS::Get()->Mount("shaders", "shaders");

		// PushLayer(new Test2D());
		PushLayer(new Test3D());
		// PushLayer(new SponzaTest());
		// PushLayer(new DeferredTest()); // Doesn't work atm
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}