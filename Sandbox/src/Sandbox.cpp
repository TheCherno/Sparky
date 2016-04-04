#include <Sparky.h>

#include "Test2D.h"
#include "Test3D.h"
#include "DeferredTest.h"
#include "Sponza.h"

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
		// PushLayer(new DeferredTest());
		//PushLayer(new Test3D());
		PushLayer(new Sponza());
		// PushLayer(new Test2D());
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}