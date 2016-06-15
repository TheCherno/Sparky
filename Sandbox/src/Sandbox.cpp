#include <Sparky.h>

#include "Test2D.h"
#include "Test3D.h"
#include "DeferredTest.h"
#include "SponzaTest.h"
#include "ScriptingTest.h"

using namespace sp;
using namespace graphics;
using namespace maths;

class Game : public Application
{
public:
	Game()
		: Application("Sandbox", { 1280, 720, false, false }, sp::graphics::API::RenderAPI::DIRECT3D)
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
		VFS::Get()->Mount("scripts", "res/scripts");

		// PushLayer(new Test2D());
		// PushLayer(new Test3D());
		// PushLayer(new SponzaTest());
		// PushLayer(new DeferredTest()); // Doesn't work atm
		PushLayer(new ScriptingTest());
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}