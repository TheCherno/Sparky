#include "sp/sp.h"
#include "DebugMenu.h"

namespace sp { namespace debug {

	DebugMenu* DebugMenu::s_Instance = nullptr;

	DebugMenu::DebugMenu()
		: m_Visible(false)
	{

	}

	void DebugMenu::Init()
	{
		s_Instance = new DebugMenu();
	}

	void DebugMenu::Add()
	{

	}

	bool DebugMenu::IsVisible()
	{
		return s_Instance->m_Visible;
	}

	void DebugMenu::SetVisible(bool visible)
	{
		s_Instance->m_Visible = visible;
	}

} }