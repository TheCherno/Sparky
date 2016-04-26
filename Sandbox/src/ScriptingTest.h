#pragma once

#include <Sparky.h>

class ScriptingTest : public sp::graphics::Layer3D
{
private:
	sp::graphics::Camera* m_MayaCameras;
	sp::graphics::Camera* m_FPSCameras;

	float m_Rotations;
	bool m_SetUniformss[2];
	std::vector<sp::graphics::PBRMaterial*> m_Materialss;
public:
	ScriptingTest();
	~ScriptingTest();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate() override;
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer3D& renderer);
};