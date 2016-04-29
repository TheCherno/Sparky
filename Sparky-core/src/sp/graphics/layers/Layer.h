#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "../../events/Event.h"
#include "../../events/IEventListener.h"
#include "sp/app/Window.h"

namespace sp { namespace graphics {

	enum class SP_API LayerType
	{
		NONE = 0, LAYER2D, LAYER3D
	};

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
		virtual void OnUpdate();
		virtual void OnRender();
		virtual LayerType GetLayerType() const { return LayerType::NONE; }
	protected:
		virtual bool OnResize(uint width, uint height);
	};

} }