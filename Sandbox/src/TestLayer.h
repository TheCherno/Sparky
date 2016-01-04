#pragma once

#include <sp/graphics/layers/Layer2D.h>
#include <sp/graphics/Label.h>
#include <sp/graphics/Renderer2D.h>

#include <sp/events/Events.h>

class TestLayer : public sp::graphics::Layer2D
{
private:
	sp::graphics::Label** debugInfo;
	sp::graphics::Renderer2D* m_Renderer;
public:
	TestLayer();
	~TestLayer();

	void OnInit(sp::graphics::Renderer2D& renderer, sp::graphics::Shader& shader) override;

	void OnTick() override;
	void OnUpdate() override;

	bool OnKeyPressedEvent(sp::events::KeyPressedEvent& event);
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer2D& renderer) override;
};