#pragma once

#include <sp/app/Application.h>
#include <sp/maths/maths.h>

#include <sp/graphics/MeshFactory.h>
#include <sp/graphics/layers/Layer3D.h>
#include <sp/graphics/shaders/ShaderFactory.h>
#include <sp/graphics/Model.h>

class Test3D : public sp::graphics::Layer3D
{
private:
	sp::graphics::Material* m_Material;
	float m_Rotation;
public:
	Test3D();
	~Test3D();

	void OnInit(sp::graphics::Renderer3D& renderer, sp::graphics::Scene& scene) override;

	void OnTick() override;
	void OnUpdate() override;
	bool OnEvent(const sp::events::Event& event) override;
};