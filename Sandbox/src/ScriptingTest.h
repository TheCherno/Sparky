#pragma once

#include <Sparky.h>

class ScriptingTest : public sp::graphics::Layer3D
{
private:
	sp::graphics::Camera* m_MayaCamera;
	sp::graphics::Camera* m_FPSCamera;

	float m_Rotation;
	bool m_SetUniforms[2];
	std::vector<sp::graphics::PBRMaterial*> m_Materials;
	sp::graphics::MaterialInstance* m_SkyboxMaterial;

	enum Materials : uint
	{
		CAST_IRON = 0,
		WORN_WOOD,
		GUN_METAL,
		ABS_RED,
		CUSTOM
	};
public:
	ScriptingTest();
	~ScriptingTest();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate() override;
	void OnEvent(sp::events::Event& event) override;

	void OnRender(sp::graphics::Renderer3D& renderer);
};