#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "../../events/Event.h"
#include "../../events/IEventListener.h"
#include "../Window.h"

namespace sp { namespace graphics {

	class SP_API Layer : public events::IEventListener
	{
	protected:
		Window* m_Window;
	public:
		Layer();
		virtual ~Layer();

		virtual void Init();
		virtual void OnEvent(events::Event& event);
		virtual void OnTick();
		virtual void OnUpdate();
		virtual void OnRender();
	};

} }