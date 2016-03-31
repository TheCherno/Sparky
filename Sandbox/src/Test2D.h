#pragma once

#include <Sparky.h>

class Test2D : public sp::graphics::Layer2D
{
private:
	sp::graphics::Label** debugInfo;
	sp::graphics::Renderer2D* m_Renderer;
public:
	Test2D();
	~Test2D();

	void OnInit(sp::graphics::Renderer2D& renderer, sp::graphics::Material& material) override;

	void OnTick() override;
	void OnUpdate() override;

	bool OnKeyPressedEvent(sp::events::KeyPressedEvent& event);
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer2D& renderer) override;
};