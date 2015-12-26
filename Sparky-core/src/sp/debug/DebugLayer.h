#pragma once

#include "sp/graphics/Renderer2D.h"
#include "sp/graphics/layers/Layer2D.h"
#include "sp/graphics/shaders/Shader.h"

#include "sp/events/Events.h"

namespace sp { namespace debug {

	class DebugLayer : public graphics::Layer2D
	{
	private:

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