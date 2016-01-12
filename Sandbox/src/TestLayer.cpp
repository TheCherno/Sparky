#include "TestLayer.h"

#include <sp/app/Application.h>

#include <sp/graphics/BatchRenderer2D.h>
#include <sp/graphics/shaders/ShaderFactory.h>
#include <sp/graphics/FontManager.h>
#include <sp/graphics/Sprite.h>

using namespace sp;
using namespace graphics;
using namespace events;
using namespace maths;

TestLayer::TestLayer()
	: Layer2D(ShaderFactory::DefaultShader(), mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
{
	m_Renderer = nullptr;
}

TestLayer::~TestLayer()
{

}

void TestLayer::OnInit(Renderer2D& renderer, Shader& shader)
{
	// m_Window->SetVsync(false);
	m_Renderer = &renderer;

	renderer.SetRenderTarget(RenderTarget::BUFFER);
	renderer.AddPostEffectsPass(new PostEffectsPass(Shader::FromFile("Horizontal Blur", "shaders/postfx.shader")));
	renderer.SetPostEffects(false);

	Texture::SetFilter(TextureFilter::NEAREST);
	Add(new Sprite(0.0f, 0.0f, 8, 8, new Texture("Tex", "res/tb.png")));
	Add(new Sprite(-8.0f, -8.0f, 6, 6, 0xffff00ff));

	debugInfo = new Label*[10];
	debugInfo[0] = new Label("", -15.5f, 6.8f, 0xffffffff);
	debugInfo[1] = new Label("", -15.5f, 5.8f, 0xffffffff);
	Add(debugInfo[0]);
	Add(debugInfo[1]);

	Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
	Mask* mask = new Mask(new Texture("Mask", "res/mask.png"));
	mask->transform = mat4::Translate(vec3(-16.0f, -9.0f, 0.0f)) * mat4::Scale(vec3(32, 18, 1));
	SetMask(mask);
}

void TestLayer::OnTick()
{
	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
}

void TestLayer::OnUpdate()
{
}

bool TestLayer::OnKeyPressedEvent(KeyPressedEvent& event)
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

void TestLayer::OnEvent(sp::events::Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestLayer::OnKeyPressedEvent));
}

void TestLayer::OnRender(Renderer2D& renderer)
{
	debugInfo[0]->text = String("Target: ") + (renderer.GetRenderTarget() == RenderTarget::SCREEN ? "Screen" : "Buffer");
	debugInfo[1]->text = String("PostFX: ") + (renderer.GetPostEffects() ? "On" : "Off");
}
