#pragma once

#include "sp/sp.h"
#include "sp/String.h"

#include "Widget.h"

#define DEFAULT_HEIGHT 0.575

namespace sp { namespace graphics { namespace ui {

	class TextField : public Widget
	{
	protected:
		String m_Label;
		bool m_State;
		int m_Cursor;
		Font* m_Font;
		float m_Padding;
		int m_SelectStart;
		uint m_SelectionColor;

		int m_UpperLimit;
		int m_LowerLimit;

		int FindMid(float x);
		void ReplaceSelection(String text);
	public:
		TextField(const maths::Rectangle& bounds, const String& text = "");
		
		bool OnMousePressed(events::MousePressedEvent& e) override;
		bool OnMouseReleased(events::MouseReleasedEvent& e) override;
		bool OnMouseMoved(events::MouseMovedEvent& e) override;

		bool OnKeyPressed(events::KeyPressedEvent& e) override;

		virtual void OnUpdate() override;
		virtual void OnRender(Renderer2D& renderer) override;

		inline void SetLabel(const String& label) { m_Label = label; }
		inline const String& GetLabel() const { return m_Label; }

		inline void SetFont(Font* font) { m_Font = font; }
		inline const Font& GetFont() const { return *m_Font; }
		
		inline void SetSelectionColor(uint color) { m_SelectionColor = color; }
		inline const uint GetSelectionColor() const { return m_SelectionColor; }
	};

} } }