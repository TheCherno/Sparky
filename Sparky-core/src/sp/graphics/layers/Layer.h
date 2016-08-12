#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "../../events/Event.h"
#include "../../events/IEventListener.h"

#include "sp/app/Window.h"
#include "sp/utils/Timestep.h"

namespace sp { namespace graphics {

	class SP_API Layer : public events::IEventListener
	{
	protected:
		Window* m_Window;
		bool m_Visible;
	public:
		Layer();
		virtual ~Layer();

		inline bool IsVisible() const { return m_Visible; }
		inline void SetVisible(bool visible) { m_Visible = visible; }

		virtual void Init();
		virtual void OnEvent(events::Event& event);
		virtual void OnTick();
		virtual void OnUpdate(const Timestep& ts);
		virtual void OnUpdateInternal(const Timestep& ts);
		virtual void OnRender();
	protected:
		virtual bool OnResize(uint width, uint height);
	};

} }