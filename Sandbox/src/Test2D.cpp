#include "Test2D.h"

using namespace sp;
using namespace graphics;
using namespace events;
using namespace entity;
using namespace component;
using namespace maths;
using namespace API;

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
	Add(spnew Sprite(4.0f, 4.0f, 4, 4, Texture2D::CreateFromFile("Tex", "res/tb.png", params)));
	Add(spnew Sprite(-5.0f, -5.0f, 3, 3, 0xffff00ff));

	FontManager::Add(spnew Font("Consolas", "res/consola.ttf", 96));
	FontManager::Add(spnew Font("Brush Script", "res/BrushScriptStd.otf", 96));

	debugInfo = spnew Label*[10];
	debugInfo[0] = spnew Label("", -15.5f, 8.5f, 0xffffffff);
	debugInfo[1] = spnew Label("", -15.5f, 7.5f, 0xffffffff);
	debugInfo[2] = spnew Label("", -15.5f, 6.5f, 0xffffffff);
	debugInfo[3] = spnew Label("", -15.5f, 5.5f, 0xffffffff);
	debugInfo[4] = spnew Label("", -15.5f, 4.5f, 0xffffffff);
	Add(debugInfo[0]);
	Add(debugInfo[1]);
	Add(debugInfo[2]);
	Add(debugInfo[3]);
	Add(debugInfo[4]);

	Add(spnew Label("Consolas", -15.5f, 0.0f, FontManager::Get("Consolas"), 0xffffffff));
	Add(spnew Label("Brush Script", -15.5f, 2.0f, FontManager::Get("Brush Script"), 0xffffffff));

	Texture::SetWrap(TextureWrap::CLAMP_TO_BORDER);
	Mask* mask = spnew Mask(Texture2D::CreateFromFile("Mask", "res/mask.png"));
	mask->transform = mat4::Translate(vec3(-16.0f, -9.0f, 0.0f)) * mat4::Scale(vec3(32, 18, 1));
	SetMask(mask);
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
