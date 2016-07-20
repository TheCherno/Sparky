#pragma once

#include <Sparky.h>

class Test3D : public sp::graphics::Layer3D
{
private:
	sp::graphics::Camera* m_MayaCamera;
	sp::graphics::Camera* m_FPSCamera;

	sp::entity::Entity* m_Cube;
	std::vector<sp::entity::Entity*> m_Spheres;
	sp::entity::Entity* m_Plane;
	sp::entity::Entity* m_Dagger;
	sp::graphics::PBRMaterial* m_DaggerMaterial;
	sp::graphics::MaterialInstance* m_SkyboxMaterial;
	sp::graphics::Light* m_Light;
	float m_Rotation;
	bool m_SetUniforms[2];
	std::vector<sp::graphics::PBRMaterial*> m_Materials;
public:
	Test3D();
	~Test3D();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate(const sp::Timestep& ts) override;
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer3D& renderer);
};