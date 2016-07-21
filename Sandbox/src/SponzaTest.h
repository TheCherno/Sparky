#pragma once

#include <Sparky.h>

class SponzaTest : public sp::graphics::Layer3D
{
private:
	sp::graphics::Camera* m_MayaCamera;
	sp::graphics::Camera* m_FPSCamera;
	sp::graphics::MaterialInstance* m_SkyboxMaterial;

public:
	SponzaTest();
	~SponzaTest();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate(const sp::Timestep& ts) override;
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer3D& renderer);
};