#pragma once

#include <Sparky.h>

class DeferredTest : public sp::graphics::Layer3D
{
private:
	sp::graphics::Scene* m_Scene;
	sp::entity::Entity* m_Cube;
	sp::entity::Entity* m_Sphere;
	sp::entity::Entity* m_Plane;
	float m_Rotation;
	bool m_SetUniforms[2];
public:
	DeferredTest();
	~DeferredTest();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate(const sp::Timestep& ts) override;
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer3D& renderer);
};