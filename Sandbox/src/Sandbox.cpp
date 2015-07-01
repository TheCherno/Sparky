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
	maths::vec3 mask;
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
		window = createWindow("Test Game", 960, 540);
		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
#ifdef SPARKY_PLATFORM_WEB
		shader = new Shader("res/shaders/basic.es3.vert", "res/shaders/basic.es3.frag");
#else
		shader = ShaderFactory::DefaultShader();
#endif
		layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		sprite = new Sprite(0.0f, 0.0f, 4, 4, new Texture("Tex", "res/tb.png"));
		layer->add(new Sprite(-16.0f, -9.0f, 32, 32, 0xffff00ff));
		layer->add(sprite);

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->add(fps);

		Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
		layer->setMask(new Texture("Mask", "res/mask.png"));

		shader->enable();
		shader->setUniformMat4("mask_matrix", maths::mat4::translation(mask));

//		audio::SoundManager::add(new audio::Sound("Lol", "res/Cherno.ogg"))->loop();
	}

	void tick() override
	{
		fps->text = std::to_string(getFPS()) + " fps";
		SPARKY_INFO(getUPS(), " ups, ", getFPS(), " fps");
	}

	void update() override
	{
		float speed = 0.01f;
		if (window->isKeyPressed(GLFW_KEY_UP))
			sprite->position.y += speed;
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
			sprite->position.y -= speed;
		if (window->isKeyPressed(GLFW_KEY_LEFT))
			sprite->position.x -= speed;
		else if (window->isKeyPressed(GLFW_KEY_RIGHT))
			sprite->position.x += speed;

		if (window->isKeyPressed(GLFW_KEY_UP))
			mask.y += speed;
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
			mask.y -= speed;
		if (window->isKeyPressed(GLFW_KEY_LEFT))
			mask.x -= speed;
		else if (window->isKeyPressed(GLFW_KEY_RIGHT))
			mask.x += speed;

		static maths::vec3 scale(1, 1, 1);
		if (window->isKeyPressed(GLFW_KEY_W))
		{
			scale.x += speed;
			scale.y += speed;
		}
		else if (window->isKeyPressed(GLFW_KEY_S))
		{
			scale.x -= speed;
			scale.y -= speed;
		}

		maths::vec2 mouse = window->getMousePosition();
		shader->setUniformMat4("mask_matrix", maths::mat4::translation(mask) * maths::mat4::scale(scale));
		// shader->setUniform2f("light_pos", maths::vec2((float)(mouse.x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - mouse.y * 18.0f / window->getHeight())));
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