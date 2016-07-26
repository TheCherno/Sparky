#pragma once

#include "sp/sp.h"

#include "sp/graphics/ui/Button.h"
#include "sp/maths/maths.h"

namespace sp { namespace debug {

	struct IAction;
	

	class DebugMenuItem : public graphics::ui::Button
	{
	protected:
		IAction* m_Action;
	public:
		DebugMenuItem(IAction* action, const maths::Rectangle& bounds);

		void OnUpdate() override;

		inline const IAction* GetAction() const { return m_Action; }
	};

} }