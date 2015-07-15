#include <sparky.h>

using namespace sparky;
using namespace graphics;

class Game : public Sparky
{
private:
	Window* window;
	Layer* layer;
	Label* fps;
	Sprite* sprite;
	Shader* shader;
	Mask* mask;
	Label* debugInfo;
public:
	Game()
	{

	}

	~Game()
	{
		delete layer;
	}

	void init() override
	{
		window = createWindow("Test Game", 1280, 720);
		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
#ifdef SPARKY_PLATFORM_WEB
		shader = new Shader("res/shaders/basic.es3.vert", "res/shaders/basic.es3.frag");
#else
		shader = ShaderFactory::DefaultShader();
#endif
		layer = new Layer(new BatchRenderer2D(maths::tvec2<uint>(1280, 720)), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		
		sprite = new Sprite(0.0f, 0.0f, 8, 4, new Texture("Tex", "res/tb.png"));
		//layer->add(new Sprite(-16.0f, -9.0f, 32, 32, 0xffff00ff));
		layer->add(sprite);

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->add(fps);

		debugInfo = new Label("", -15.5f, 6.8f, 0xffffffff);
		layer->add(debugInfo);

		Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
		mask = new Mask(new Texture("Mask", "res/mask.png"));
		layer->setMask(mask);

//		audio::SoundManager::add(new audio::Sound("Lol", "res/Cherno.ogg"))->loop();
	}

	void tick() override
	{
		fps->text = std::to_string(getFPS()) + " fps";
		SPARKY_INFO(getUPS(), " ups, ", getFPS(), " fps");
	}

	void update() override
	{
		if (window->isKeyPressed(GLFW_KEY_1))
			((BatchRenderer2D*)layer->renderer)->SetRenderTarget(RenderTarget::SCREEN);
		if (window->isKeyPressed(GLFW_KEY_2))
			((BatchRenderer2D*)layer->renderer)->SetRenderTarget(RenderTarget::BUFFER);

		maths::tvec2<uint> size = ((BatchRenderer2D*)layer->renderer)->GetViewportSize();

		if (window->isKeyPressed(GLFW_KEY_UP))
		{
			size.x++;
			size.y++;
		}
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
		{
			size.x--;
			size.y--;
		}
		debugInfo->text = std::to_string(size.x) + ", " + std::to_string(size.y);
		((BatchRenderer2D*)layer->renderer)->SetViewportSize(size);
	}

	void render() override
	{
		layer->render();
	}
};

int main()
{
	Game game;
	game.start();
	return 0;
}