#include "sp/sp.h"
#include "DebugLayer.h"

#include "sp/maths/maths.h"
#include "sp/graphics/shaders/ShaderFactory.h"
#include "sp/graphics/Label.h"
#include "sp/graphics/Sprite.h"

#include "sp/embedded/Embedded.h"

namespace sp { namespace debug {

	using namespace maths;
	using namespace graphics;
	using namespace events;

	DebugLayer::DebugLayer()
		: Layer2D(ShaderFactory::DefaultShader(), mat4::Orthographic(0.0f, 32.0f, 0.0f, 18.0f, -1.0f, 1.0f)), m_Application(Application::GetApplication())
	{
	}

	DebugLayer::~DebugLayer()
	{
	}

	void DebugLayer::OnInit(graphics::Renderer2D& renderer, graphics::Shader& shader)
	{
		renderer.SetRenderTarget(RenderTarget::SCREEN);
		m_FPSLabel = new Label("", 30.0f, 17.2f, FontManager::Get(24), 0xffffffff);
		Add(m_FPSLabel);

		DebugMenu::Add("Example Item");
		DebugMenu::Add("This is another example");
	}

	void DebugLayer::OnTick()
	{
		m_FPSLabel->text = std::to_string(m_Application.GetFPS()) + " fps";
	}

	void DebugLayer::OnUpdate()
	{
	}

	void DebugLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(METHOD(&DebugLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(METHOD(&DebugLayer::OnKeyPressedEvent));
	}

	bool DebugLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		return false;
	}

	bool DebugLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		if (event.GetRepeat())
			return false;

		if (event.GetModifiers() == SP_MODIFIER_LEFT_CONTROL && event.GetKeyCode() == SP_KEY_TAB)
		{
			DebugMenu::SetVisible(!DebugMenu::IsVisible());
			return true;
		}
		return false;
	}

	void DebugLayer::OnRender(graphics::Renderer2D& renderer)
	{
		if (DebugMenu::IsVisible())
			DebugMenu::OnRender(renderer);
	}

} }