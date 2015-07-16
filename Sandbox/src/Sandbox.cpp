#include <sparky.h>

using namespace sparky;
using namespace graphics;
using namespace maths;

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

	void Init() override
	{
		window = CreateWindow("Test Game", 1280, 720);
		// window->SetVsync(false);

		FontManager::Get()->SetScale(window->GetWidth() / 32.0f, window->GetHeight() / 18.0f);
		shader = ShaderFactory::DefaultShader();
		layer = new Layer(new BatchRenderer2D(tvec2<uint>(1280, 720)), shader, mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		
		sprite = new Sprite(0.0f, 0.0f, 8, 8, new Texture("Tex", "res/tb.png"));
		layer->Add(sprite);

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->Add(fps);

		debugInfo = new Label("", -15.5f, 6.8f, 0xffffffff);
		layer->Add(debugInfo);

		Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
		mask = new Mask(new Texture("Mask", "res/mask.png"));
		mask->transform = mat4::Translate(vec3(-16.0f, -9.0f, 0.0f)) * mat4::Scale(vec3(32, 18, 1));
		layer->SetMask(mask);
	}

	void Tick() override
	{
		fps->text = std::to_string(GetFPS()) + " fps";
		SPARKY_INFO(GetUPS(), " ups, ", GetFPS(), " fps");
	}

	void Update() override
	{
		if (window->IsKeyPressed(GLFW_KEY_1))
			((BatchRenderer2D*)layer->renderer)->SetRenderTarget(RenderTarget::SCREEN);
		if (window->IsKeyPressed(GLFW_KEY_2))
			((BatchRenderer2D*)layer->renderer)->SetRenderTarget(RenderTarget::BUFFER);

		tvec2<uint> size = ((BatchRenderer2D*)layer->renderer)->GetViewportSize();

		if (window->IsKeyPressed(GLFW_KEY_UP))
		{
			size.x += 16;
			size.y += 9;
		}
		else if (window->IsKeyPressed(GLFW_KEY_DOWN))
		{
			size.x -= 16;
			size.y -= 9;
		}

		if (size.x > 10000)
			size.x = 0;
		if (size.y > 10000)
			size.y = 0;

		debugInfo->text = std::to_string(size.x) + ", " + std::to_string(size.y);
		((BatchRenderer2D*)layer->renderer)->SetViewportSize(size);
		((BatchRenderer2D*)layer->renderer)->SetScreenSize(tvec2<uint>(window->GetWidth(), window->GetHeight()));
	}

	void Render() override
	{
		layer->Render();
	}
};

int main()
{
	Game game;
	game.Start();
	return 0;
}