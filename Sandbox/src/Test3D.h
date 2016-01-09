#pragma once

#include <Sparky.h>

class Test3D : public sp::graphics::Layer3D
{
private:
	sp::entity::Entity* m_Cube;
	sp::entity::Entity* m_Sphere;
	sp::entity::Entity* m_Plane;
	float m_Rotation;
	bool m_SetUniforms[2];
public:
	Test3D();
	~Test3D();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate() override;
	void OnEvent(sp::events::Event& event) override;
};