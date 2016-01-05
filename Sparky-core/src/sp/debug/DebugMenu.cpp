#include "sp/sp.h"
#include "DebugMenu.h"

namespace sp { namespace debug {

	using namespace maths;

	DebugMenu* DebugMenu::s_Instance = nullptr;

	DebugMenu::DebugMenu()
		: m_Visible(false), m_Padding(1.5f), m_FontSize(24.0f)
	{
	}

	void DebugMenu::Init()
	{
		s_Instance = new DebugMenu();
	}

	void DebugMenu::Add(const String& name)
	{
		s_Instance->m_DebugMenuItems.push_back(new IAction(name));
	}

	void DebugMenu::Add(const String& name, float* value)
	{
		s_Instance->m_DebugMenuItems.push_back(new FloatAction(name, [value]() { return *value; }, [value](float v) { *value = v; }));
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
		float yOffset = s_Instance->m_Padding;
		for (IAction* item : s_Instance->m_DebugMenuItems)
		{
			float y = 18.0f - yOffset;
			renderer.FillRect(0, y, 4 + s_Instance->m_Padding, s_Instance->m_Padding, 0xcf7f7f7f);
			renderer.DrawString(item->ToString(), vec3(0.2f, y + s_Instance->m_Padding / 2.0f), *graphics::FontManager::Get(s_Instance->m_FontSize));

			yOffset += s_Instance->m_Padding;
		}

	}

} }