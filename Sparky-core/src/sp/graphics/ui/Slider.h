#pragma once

#include "sp/sp.h"
#include "sp/maths/maths.h"

#include "Widget.h"

namespace sp { namespace graphics { namespace ui {

	class SP_API Slider : public Widget
	{
	public:
		using ValueChangedCallback = std::function<void(float)>;
	private:
		enum class SliderState
		{
			UNPRESSED, PRESSED, PRESSEDHEAD
		};
	private:
		maths::Rectangle m_HeadBounds;
		float m_Value;
		float m_HeadOffset;
		SliderState m_State;
		ValueChangedCallback m_Callback;
		bool m_Vertical;
	public:
		Slider(const maths::Rectangle& bounds, bool vertical = false);
		Slider(const maths::Rectangle& bounds, float value = 0.0f, const ValueChangedCallback& callback = &Slider::NoCallback, bool vertical = false);

		bool OnMousePressed(events::MousePressedEvent& e) override;
		bool OnMouseReleased(events::MouseReleasedEvent& e) override;
		bool OnMouseMoved(events::MouseMovedEvent& e) override;

		void OnUpdate() override;
		void OnRender(Renderer2D& renderer) override;

		inline void SetCallback(const ValueChangedCallback& callback) { m_Callback = callback; }

		inline float GetValue() const { return m_Value; }
		void SetValue(float value);
	private:
		static void NoCallback(float) {}
	};

} } }