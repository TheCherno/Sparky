#include "../src/sparky.h"

using namespace sparky;
using namespace graphics;

class Game : public Sparky
{
private:
	Window* window;
	Layer* layer, *bg;
	Label* fps;
	Shader* shader;
	Model* model;
	float angle;
	CubeMap* cubemap;
	maths::mat4 pr_matrix, ml_matrix, vw_matrix;
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
		layer = new Layer(new BatchRenderer2D(), new Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		bg = new Layer(new BatchRenderer2D(), new Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), maths::mat4::perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->add(fps);
		bg->add(new Sprite(0, 0, 960, 540, new Texture("Skybox - tex.png")));

		shader = new Shader("src/shaders/ibl.vert", "src/shaders/ibl.frag");
		model = new Model("teapot.obj", 0.04f);
		cubemap = new CubeMap("Skybox.png", 500, 500);
		cubemap->bind();
		shader->enable();
		shader->setUniform1i("diffuseEnvMap", 0);
		pr_matrix = maths::mat4::perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		vw_matrix = maths::mat4::translation(maths::vec3(0, -2, -5));
	}

	void tick() override
	{
		fps->text = std::to_string(getFPS()) + " fps";
		std::cout << getUPS() << " ups, " << getFPS() << " fps" << std::endl;
	}

	void update() override
	{
		ml_matrix = maths::mat4::rotation(angle++, maths::vec3(0, 1, 0));
	}

	void render() override
	{
		bg->m_Shader->enable();
		bg->m_Shader->setUniformMat4("vw_matrix", maths::mat4::translation(maths::vec3(-405, -300, -350)));
		bg->m_Shader->setUniformMat4("ml_matrix", maths::mat4::translation(maths::vec3(0, 0, -5)));
		bg->render();
		shader->enable();
		shader->setUniformMat4("pr_matrix", pr_matrix);
		shader->setUniformMat4("ml_matrix", ml_matrix);
		shader->setUniformMat4("vw_matrix", vw_matrix);
		glActiveTexture(GL_TEXTURE0);
		cubemap->bind();
		model->render();
		glDisable(GL_DEPTH_TEST);
		layer->render();
		glEnable(GL_DEPTH_TEST);
	}
};

int main()
{
	Game game;
	game.start();
	return 0;
}