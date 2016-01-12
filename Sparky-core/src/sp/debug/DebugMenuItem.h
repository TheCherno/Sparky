#pragma once

#include "sp/sp.h"
#include "DebugMenuAction.h"

#include "sp/graphics/ui/Button.h"
#include "sp/maths/maths.h"

namespace sp { namespace debug {

	class DebugMenuItem : public graphics::ui::Button
	{
	private:
		IAction* m_Action;
	public:
		DebugMenuItem(IAction* action, const maths::Rectangle& bounds);

		void OnUpdate() override;
	};

} }