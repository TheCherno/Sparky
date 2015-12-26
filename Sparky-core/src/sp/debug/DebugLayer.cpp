#include "sp/sp.h"
#include "DebugLayer.h"

#include "sp/maths/maths.h"
#include "sp/graphics/shaders/ShaderFactory.h"
#include "sp/graphics/Label.h"
#include "sp/graphics/Sprite.h"

namespace sp { namespace debug {

	using namespace maths;
	using namespace graphics;
	using namespace events;

	DebugLayer::DebugLayer()
		: Layer2D(ShaderFactory::DefaultShader(), mat4::Orthographic(0.0f, 32.0f, 0.0f, 18.0f, -1.0f, 1.0f))
	{
		m_Visible = false;
	}

	DebugLayer::~DebugLayer()
	{

	}

	void DebugLayer::OnInit(graphics::Renderer2D& renderer, graphics::Shader& shader)
	{
		FontManager::Get()->SetScale(m_Window->GetWidth() / 32.0f, m_Window->GetHeight() / 18.0f);

		renderer.SetRenderTarget(RenderTarget::SCREEN);
		for (int i = 0; i < 5; i++)
		{
			float y = 18.0f - (i + 1) * 1.7f;
			Add(new Sprite(0, y, 6, 1.5f, 0xcf7f7f7f));
			Add(new Label(String("Item ") + std::to_string(i + 1), 0.2f, y + 0.4f, 0xffffffff));
		}
	}

	void DebugLayer::OnTick()
	{

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

		if (event.GetModifiers() == SP_MODIFIER_LEFT_CONTROL && event.GetKeyCode() == VK_TAB)
		{
			m_Visible = !m_Visible;
			return true;
		}
		return false;
	}

	void DebugLayer::OnRender(graphics::Renderer2D& renderer)
	{

	}

} }