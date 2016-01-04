#pragma once

#include "sp/graphics/Renderer2D.h"
#include "sp/graphics/Label.h"
#include "sp/graphics/Sprite.h"

#include "sp/graphics/layers/Layer2D.h"
#include "sp/graphics/shaders/Shader.h"

#include "sp/events/Events.h"
#include "sp/app/Application.h"

#include "sp/debug/DebugMenu.h"

namespace sp { namespace debug {

	class DebugLayer : public graphics::Layer2D
	{
	private:
		Application& m_Application;
		graphics::Label* m_FPSLabel;
	public:
		DebugLayer();
		~DebugLayer();

		void OnInit(graphics::Renderer2D& renderer, graphics::Shader& shader) override;

		void OnTick() override;
		void OnUpdate() override;

		void OnEvent(events::Event& event) override;
		bool OnMouseMovedEvent(events::MouseMovedEvent& event);
		bool OnKeyPressedEvent(events::KeyPressedEvent& event);

		void OnRender(graphics::Renderer2D& renderer) override;
	};

} }