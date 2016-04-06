#pragma once

#include <Sparky.h>

class ShadowTest : public sp::graphics::Layer3D
{
private:
	sp::graphics::Camera* m_MayaCamera;
	sp::graphics::Camera* m_FPSCamera;
	sp::graphics::MaterialInstance* m_SkyboxMaterial;

	sp::entity::Entity* m_Cube;
	sp::maths::vec3 m_CubePosition;
	bool m_ShowShadowMap;
public:
	ShadowTest();
	~ShadowTest();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate() override;
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer3D& renderer);
};