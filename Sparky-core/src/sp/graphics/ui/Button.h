#pragma once

#include "sp/sp.h"
#include "sp/String.h"

#include "Widget.h"

namespace sp { namespace graphics { namespace ui {

	class Button : public Widget
	{
	public:
		using ActionHandler = std::function<void()>;
	private:
		enum class ButtonState
		{
			UNPRESSED, PRESSED
		};
	protected:
		String m_Label;
		ButtonState m_State;
		ActionHandler m_ActionHandler;
		Font* m_Font;
	public:
		Button(const String& label, const maths::Rectangle& bounds, const ActionHandler& handler = &Button::NoAction);

		bool OnMousePressed(events::MousePressedEvent& e) override;
		bool OnMouseReleased(events::MouseReleasedEvent& e) override;
		bool OnMouseMoved(events::MouseMovedEvent& e) override;

		virtual void OnAction();
		virtual void OnUpdate() override;
		virtual void OnRender(Renderer2D& renderer) override;

		inline void SetLabel(const String& label) { m_Label = label; }
		inline const String& GetLabel() const { return m_Label; }

		inline void SetFont(Font* font) { m_Font = font; }
		inline const Font& GetFont() const { return *m_Font; }

		inline void SetAction(const ActionHandler& action) { m_ActionHandler = action; }
	private:
		static void NoAction() {}
	};


} } }