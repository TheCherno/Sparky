#include <sparky.h>
#include <app/Application.h>

using namespace sparky;
using namespace graphics;
using namespace maths;

#define WIDTH 1280
#define HEIGHT 720

class Game : public Application
{
private:
	Layer* layer;
	Label* fps;
	Sprite* sprite;
	Shader* shader;
	Mask* mask;
	Label** debugInfo;
public:
	Game()
		: Application("Test Game", WIDTH, HEIGHT)
	{

	}

	~Game()
	{
		delete layer;
	}

	void Init() override
	{
		Application::Init();
		// window->SetVsync(false);

		FontManager::Get()->SetScale(window->GetWidth() / 32.0f, window->GetHeight() / 18.0f);
		shader = ShaderFactory::DefaultShader();
		layer = new Layer(new BatchRenderer2D(tvec2<uint>(WIDTH, HEIGHT)), shader, mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		layer->renderer->SetRenderTarget(RenderTarget::BUFFER);
		layer->renderer->AddPostEffectsPass(new PostEffectsPass(Shader::FromFile("shaders/postfx.vert", "shaders/postfx.frag")));
		layer->renderer->SetPostEffects(true);

		Texture::SetFilter(TextureFilter::NEAREST);
		sprite = new Sprite(0.0f, 0.0f, 8, 8, new Texture("Tex", "res/tb.png"));
		layer->Add(sprite);
		layer->Add(new Sprite(-8.0f, -8.0f, 6, 6, 0xffff00ff));

		fps = new Label("", -15.5f, 7.8f, 0xffffffff);
		layer->Add(fps);

		debugInfo = new Label*[10];
		debugInfo[0] = new Label("", -15.5f, 6.8f, 0xffffffff);
		debugInfo[1] = new Label("", -15.5f, 5.8f, 0xffffffff);
		layer->Add(debugInfo[0]);
		layer->Add(debugInfo[1]);

		Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
		mask = new Mask(new Texture("Mask", "res/mask.png"));
		mask->transform = mat4::Translate(vec3(-16.0f, -9.0f, 0.0f)) * mat4::Scale(vec3(32, 18, 1));
		// layer->SetMask(mask);
	}

	void Tick() override
	{
		fps->text = std::to_string(GetFPS()) + " fps";
		SPARKY_INFO(GetUPS(), " ups, ", GetFPS(), " fps");
	}

	void Update() override
	{
		if (window->IsKeyTyped(VK_T))
			layer->renderer->SetRenderTarget(layer->renderer->GetRenderTarget() == RenderTarget::SCREEN ? RenderTarget::BUFFER : RenderTarget::SCREEN);
		if (window->IsKeyTyped(VK_P))
			layer->renderer->SetPostEffects(!layer->renderer->GetPostEffects());

		debugInfo[0]->text = std::string("Target: ") + (layer->renderer->GetRenderTarget() == RenderTarget::SCREEN ? "Screen" : "Buffer");
		debugInfo[1]->text = std::string("PostFX: ") + (layer->renderer->GetPostEffects() ? "On" : "Off");
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