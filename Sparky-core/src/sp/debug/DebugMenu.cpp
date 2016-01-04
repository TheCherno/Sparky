#include "sp/sp.h"
#include "DebugMenu.h"

namespace sp { namespace debug {

	using namespace maths;

	DebugMenu* DebugMenu::s_Instance = nullptr;

	DebugMenu::DebugMenu()
		: m_Visible(false)
	{

	}

	void DebugMenu::Init()
	{
		s_Instance = new DebugMenu();
	}

	void DebugMenu::Add(const String& name)
	{
		s_Instance->m_DebugMenuItems.push_back({ name });
	}

	bool DebugMenu::IsVisible()
	{
		return s_Instance->m_Visible;
	}

	void DebugMenu::SetVisible(bool visible)
	{
		s_Instance->m_Visible = visible;
	}

	void DebugMenu::OnRender(graphics::Renderer2D& renderer)
	{
		float yOffset = 1.7f;
		for (DebugMenuItem& item : s_Instance->m_DebugMenuItems)
		{
			float y = 18.0f - yOffset;
			renderer.FillRect(0, y, 6, 1.5f, 0xcf7f7f7f);
			renderer.DrawString(item.name, vec3(0.2f, y + 0.4f));

			yOffset += 1.7f;
		}

	}

} }