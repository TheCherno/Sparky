#pragma once

#include "sp/sp.h"
#include "sp/graphics/Renderer2D.h"
#include "sp/graphics/layers/Layer2D.h"

namespace sp { namespace graphics { namespace ui {

	class Widget;

	class Panel : public Layer2D
	{
	private:
		std::vector<Widget*> m_Widgets;
	public:
		Panel();
		~Panel();

		Widget* Add(Widget* widget);
		void Remove(Widget* widget);
		void Clear();

		void OnEvent(events::Event& e) override;
		bool OnMousePressedEvent(events::MousePressedEvent& e);
		bool OnMouseReleasedEvent(events::MouseReleasedEvent& e);
		bool OnMouseMovedEvent(events::MouseMovedEvent& e);

		void OnUpdate() override;
		void OnRender(Renderer2D& renderer) override;
	};

} } }