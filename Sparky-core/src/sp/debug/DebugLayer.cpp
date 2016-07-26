#include "sp/sp.h"
#include "DebugLayer.h"

#include "sp/maths/maths.h"
#include "sp/graphics/shaders/ShaderFactory.h"
#include "sp/graphics/Label.h"
#include "sp/graphics/Sprite.h"
#include "sp/system/MemoryManager.h"

#include "sp/embedded/Embedded.h"

namespace sp { namespace debug {

	using namespace maths;
	using namespace graphics;
	using namespace events;
	using namespace internal;

	DebugLayer* DebugLayer::s_Instance = nullptr;

	DebugLayer::DebugLayer()
		: Layer2D(mat4::Orthographic(0.0f, 32.0f, 0.0f, 18.0f, -1.0f, 1.0f)), m_Application(Application::GetApplication())
	{
		s_Instance = this;
	}

	DebugLayer::~DebugLayer()
	{
	}

	void DebugLayer::OnInit(graphics::Renderer2D& renderer, graphics::Material& material)
	{
		renderer.SetRenderTarget(RenderTarget::SCREEN);
		m_FPSLabel = spnew Label("", 31.5f, 17.5f, FontManager::Get(24), 0xffffffff, Label::Alignment::RIGHT);
		m_FrametimeLabel = spnew Label("", 31.5f, 16.8f, FontManager::Get(24), 0xffffffff, Label::Alignment::RIGHT);
		m_MemoryUsageLabel = spnew Label("", 31.5f, 16.1f, FontManager::Get(24), 0xffffffff, Label::Alignment::RIGHT);

		Add(m_FPSLabel);
		Add(m_MemoryUsageLabel);
		Add(m_FrametimeLabel);
	}

	void DebugLayer::OnTick()
	{
		m_FPSLabel->SetText(StringFormat::ToString(m_Application.GetFPS()) + " fps");
		m_MemoryUsageLabel->SetText(MemoryManager::BytesToString(MemoryManager().Get()->GetMemoryStats().currentUsed));
	}

	void DebugLayer::OnUpdate(const Timestep& ts)
	{
		DebugMenu::Get()->OnUpdate();

		m_FrametimeLabel->SetText(StringFormat::Float(m_Application.GetFrametime()) + " ms");
	}

	void DebugLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(METHOD(&DebugLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<MousePressedEvent>(METHOD(&DebugLayer::OnMousePressedEvent));
		dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&DebugLayer::OnMouseReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(METHOD(&DebugLayer::OnMouseMovedEvent));
		
		Layer::OnEvent(e);
	}

	bool DebugLayer::OnMousePressedEvent(events::MousePressedEvent& e)
	{
		return DebugMenu::IsVisible() ? DebugMenu::Get()->OnMousePressed(e) : false;
	}

	bool DebugLayer::OnMouseReleasedEvent(events::MouseReleasedEvent& e)
	{
		return DebugMenu::IsVisible() ? DebugMenu::Get()->OnMouseReleased(e) : false;
	}

	bool DebugLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetRepeat())
			return false;

		if (e.GetModifiers() == SP_MODIFIER_LEFT_CONTROL && e.GetKeyCode() == SP_KEY_TAB)
		{
			DebugMenu::SetVisible(!DebugMenu::IsVisible());
			return true;
		}
		return false;
	}

	bool DebugLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		return false;
	}

	void DebugLayer::OnRender(graphics::Renderer2D& renderer)
	{
		if (DebugMenu::IsVisible())
			DebugMenu::Get()->OnRender(renderer);

		for (uint i = 0; i < m_TempSprites.size(); i++)
			spdel m_TempSprites[i];

		m_TempSprites.clear();
	}

	void DebugLayer::DrawSprite(Sprite* sprite)
	{
		s_Instance->Submit(sprite);
	}

	void DebugLayer::DrawTexture(API::Texture* texture, const maths::vec2& position, const maths::vec2& size)
	{
		Sprite* sprite = spnew Sprite(position.x, position.y, size.x, size.y, texture);
		s_Instance->m_TempSprites.push_back(sprite);
		s_Instance->Submit(sprite);
	}

} }