#pragma once

#include "sp/sp.h"
#include "Panel.h"

#include "sp/maths/maths.h"

namespace sp { namespace graphics { namespace ui {

	class SP_API Widget
	{
	protected:
		bool m_Active;
		bool m_Focused;

		Panel* m_Panel;
		maths::Rectangle m_Bounds;
	private:
		Widget() {}
	protected:
		Widget(const maths::Rectangle& bounds);
	public:
		virtual bool OnMousePressed(events::MousePressedEvent& e);
		virtual bool OnMouseReleased(events::MouseReleasedEvent& e);
		virtual bool OnMouseMoved(events::MouseMovedEvent& e);

		virtual void OnUpdate();
		virtual void OnRender(Renderer2D& renderer);

		inline const maths::Rectangle& GetBounds() const { return m_Bounds; }
		inline void SetBounds(const maths::Rectangle& bounds) { m_Bounds = bounds; }

		inline bool IsActive() const { return m_Active; }
		inline void SetActive(bool active) { m_Active = active; }
	};

} } }