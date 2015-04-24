#include "../src/sparky.h"

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
		using namespace audio;

		window = createWindow("Test Game", 960, 540);
		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
#ifdef SPARKY_EMSCRIPTEN
		shader = new Shader("res/shaders/basic.es3.vert", "res/shaders/basic.es3.frag");
#else
		shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
#endif
		layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
#ifdef SPARKY_EMSCRIPTEN
		sprite = new Sprite(0.0f, 0.0f, 4, 4, new Texture("res/tb.png"));
#else
		sprite = new Sprite(0.0f, 0.0f, 4, 4, new Texture("tb.png"));
#endif
		layer->add(sprite);

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->add(fps);

#ifdef SPARKY_EMSCRIPTEN
		SoundManager::add(new Sound("Cherno", "res/Cherno.ogg"));
#else
		SoundManager::add(new Sound("Cherno", "Cherno.ogg"));
#endif
	}

	void tick() override
	{
		fps->text = std::to_string(getFPS()) + " fps";
		std::cout << getUPS() << " ups, " << getFPS() << " fps" << std::endl;
	}

	void update() override
	{
		float speed = 0.5f;
		if (window->isKeyPressed(GLFW_KEY_UP))
			sprite->position.y += speed;
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
			sprite->position.y -= speed;
		if (window->isKeyPressed(GLFW_KEY_LEFT))
			sprite->position.x -= speed;
		else if (window->isKeyPressed(GLFW_KEY_RIGHT))
			sprite->position.x += speed;
		
		if (window->isKeyTyped(GLFW_KEY_P))
			audio::SoundManager::get("Cherno")->play();
		if (window->isKeyTyped(GLFW_KEY_S))
			audio::SoundManager::get("Cherno")->stop();
		if (window->isKeyTyped(GLFW_KEY_L))
			audio::SoundManager::get("Cherno")->loop();
		if (window->isKeyTyped(GLFW_KEY_J))
			audio::SoundManager::get("Cherno")->pause();
		if (window->isKeyTyped(GLFW_KEY_H))
			audio::SoundManager::get("Cherno")->resume();
		if (window->isKeyTyped(GLFW_KEY_I))
			audio::SoundManager::get("Cherno")->setGain(audio::SoundManager::get("Cherno")->getGain()-0.1);
		if (window->isKeyTyped(GLFW_KEY_G))
			audio::SoundManager::get("Cherno")->setGain(audio::SoundManager::get("Cherno")->getGain()+0.1);

		double x, y;
		window->getMousePosition(x, y);
		shader->setUniform2f("light_pos", maths::vec2((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
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