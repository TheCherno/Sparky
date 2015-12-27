#pragma once

#include <sp/app/Application.h>
#include <sp/maths/maths.h>

#include <sp/graphics/MeshFactory.h>
#include <sp/graphics/layers/Layer3D.h>
#include <sp/graphics/shaders/ShaderFactory.h>
#include <sp/graphics/Model.h>

#include <sp/entity/Entity.h>
#include <sp/entity/component/Components.h>

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