#include "Test2D.h"

using namespace sp;
using namespace graphics;
using namespace events;
using namespace entity;
using namespace component;
using namespace maths;
using namespace API;

static float s_BoxSize = 0.1f;

Test2D::Test2D()
	: Layer2D(spnew Scene2D(mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)))
{
	m_Renderer = nullptr;
}

Test2D::~Test2D()
{

}

void Test2D::OnInit(Renderer2D& renderer, Material& material)
{
	// m_Window->SetVsync(false);
	m_Renderer = &renderer;

	renderer.SetRenderTarget(RenderTarget::SCREEN);
	//renderer.AddPostEffectsPass(new PostEffectsPass(Shader::CreateFromFile("Horizontal Blur", "shaders/postfx.shader")));
	//renderer.SetPostEffects(false);

	TextureParameters params(TextureFilter::NEAREST);
	Add(new Sprite(0.0f, 0.0f, 4, 4, Texture2D::CreateFromFile("Tex", "res/tb.png", params)));

	Entity* pinkSquare = spnew Entity(spnew Sprite(-8.0f, -8.0f, 3, 3, 0xffff00ff));
	pinkSquare->CreateComponent<Physics2DComponent>();
	m_Scene->Add(pinkSquare);

	FontManager::Add(new Font("Consolas", "res/consola.ttf", 96));
	FontManager::Add(new Font("Brush Script", "res/BrushScriptStd.otf", 96));

	debugInfo = new Label*[10];
	debugInfo[0] = new Label("", -15.5f, 7.8f, 0xffffffff);
	debugInfo[1] = new Label("", -15.5f, 6.8f, 0xffffffff);
	debugInfo[2] = new Label("", -15.5f, 5.8f, 0xffffffff);
	debugInfo[3] = new Label("", -15.5f, 4.8f, 0xffffffff);
	debugInfo[4] = new Label("", -15.5f, 3.8f, 0xffffffff);
	Add(debugInfo[0]);
	Add(debugInfo[1]);
	Add(debugInfo[2]);
	Add(debugInfo[3]);
	Add(debugInfo[4]);

	Add(new Label("Consolas", -15.5f, 0.0f, FontManager::Get("Consolas"), 0xffffffff));
	Add(new Label("Brush Script", -15.5f, 2.0f, FontManager::Get("Brush Script"), 0xffffffff));

	Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
	Mask* mask = new Mask(Texture2D::CreateFromFile("Mask", "res/mask.png"));
	mask->transform = mat4::Translate(vec3(-16.0f, -9.0f, 0.0f)) * mat4::Scale(vec3(32, 18, 1));
	SetMask(mask);

	for (int i = 0; i < 100; i++)
	{
		Entity* entity = spnew Entity(spnew Sprite(0.0f, 9.0f, s_BoxSize, s_BoxSize, vec4((rand() % 1000) / 1000.0f, 0.5f, 0.5f, 1.0f)));
		Physics2DComponent& p = entity->CreateComponent<Physics2DComponent>();
		m_Scene->Add(entity);

		p.ApplyForce(((rand() % 1000) - 500) * 0.005f);
	}

	debug::DebugMenu::Add("Box Size", &s_BoxSize, 0.0f, 1.0f);
}

void Test2D::OnTick()
{
	using namespace sp::internal;

	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");

	debugInfo[2]->SetText("Total Allocs: " + StringFormat::ToString(MemoryManager::Get()->GetMemoryStats().totalAllocations));
	debugInfo[3]->SetText("Total Allocated: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryStats().totalAllocated));
	debugInfo[4]->SetText("Total Freed: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryStats().totalFreed));
}

void Test2D::OnUpdate(const Timestep& ts)
{

}

bool Test2D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (!m_Renderer)
		return false;

	Renderer2D& renderer = *m_Renderer;

	if (event.GetRepeat())
		return false;

	if (event.GetKeyCode() == SP_KEY_T)
	{
		renderer.SetRenderTarget(renderer.GetRenderTarget() == RenderTarget::SCREEN ? RenderTarget::BUFFER : RenderTarget::SCREEN);
		return true;
	}
	if (event.GetKeyCode() == SP_KEY_P)
	{
		renderer.SetPostEffects(!renderer.GetPostEffects());
		return true;
	}

	return false;
}

bool Test2D::OnMousePressedEvent(MousePressedEvent& event)
{
	if (event.GetButton() == SP_MOUSE_LEFT)
	{
		vec2 pos = event.GetPosition();
		pos.x = pos.x / Application::GetApplication().GetWindowWidth() * 32.0f - 16.0f;
		pos.y = 9.0f - pos.y / Application::GetApplication().GetWindowHeight() * 18.0f;

		Entity* entity = spnew Entity(new Sprite(pos.x, pos.y, s_BoxSize, s_BoxSize, vec4((rand() % 1000) / 1000.0f, 0.5f, 0.5f, 1.0f)));
		Physics2DComponent& p = entity->CreateComponent<Physics2DComponent>();
		m_Scene->Add(entity);
		p.ApplyForce(((rand() % 1000) - 500) * 0.005f);

		return true;
	}
	return false;
}

void Test2D::OnEvent(sp::events::Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&Test2D::OnKeyPressedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&Test2D::OnMousePressedEvent));
}

void Test2D::OnRender(Renderer2D& renderer)
{
	debugInfo[0]->SetText(String("Target: ") + (renderer.GetRenderTarget() == RenderTarget::SCREEN ? "Screen" : "Buffer"));
	debugInfo[1]->SetText(String("PostFX: ") + (renderer.GetPostEffects() ? "On" : "Off"));
}
