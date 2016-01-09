#include "sp/sp.h"
#include "DebugMenuItem.h"

#include "sp/graphics/FontManager.h"

#include "DebugMenu.h"

namespace sp { namespace debug {

	using namespace graphics;

	DebugMenuItem::DebugMenuItem(IAction* action, const maths::Rectangle& bounds)
		: Button(action->ToString(), bounds, [action]() { action->OnAction(); }), m_Action(action)
	{
	}

	void DebugMenuItem::OnUpdate()
	{
		m_Label = m_Action->ToString();
		m_Font = FontManager::Get(DebugMenu::Get()->GetSettings().fontSize);
	}

} }