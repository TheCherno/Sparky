#include "Test3D.h"

using namespace sp;
using namespace graphics;
using namespace maths;

Test3D::Test3D()
	: Layer3D(new Scene())
{
	m_Rotation = 0.0f;
	m_SetUniforms[0] = true;
	m_SetUniforms[1] = true;
}

Test3D::~Test3D()
{
}

void Test3D::OnInit(Renderer3D& renderer, Scene& scene)
{
	VertexArray* va = MeshFactory::CreateQuad(0, 0, 5, 5);
	IndexBuffer* ib = new IndexBuffer(new uint[6]{ 0, 1, 2, 2, 3, 0 }, 6);

	Material* material = new Material(Shader::FromFile("Scene", "shaders/Scene.shader"));

	material->SetUniform("pr_matrix", mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
	material->SetUniform("vw_matrix", mat4::Translate(vec3(0, 0, -10.0f)));
	material->SetUniform("ml_matrix", mat4::Rotate(45.0f, vec3(0, 1, 0)));

	m_CubeMaterial = new MaterialInstance(material);
	m_SphereMaterial = new MaterialInstance(material);

	Model* cube = new Model("res/Cube.obj", m_CubeMaterial);
	Model* sphere = new Model("res/Sphere.obj", m_SphereMaterial);

	m_Scene->Add(cube->GetMesh());
	m_Scene->Add(sphere->GetMesh());
}

void Test3D::OnTick()
{
	Application& app = Application::GetApplication();
	SPARKY_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");

	// m_Cube->GetMaterialInstance()->GetMaterial()->DumpUniformData();
}

void Test3D::OnUpdate()
{
	mat4 transform = mat4::Rotate(m_Rotation, vec3(1, 0, 0)) * mat4::Rotate(m_Rotation, vec3(0, 1, 0)) * mat4::Rotate(m_Rotation, vec3(0, 0, 1));
	m_Rotation += 0.5f;

	if (m_SetUniforms[0])
		m_CubeMaterial->SetUniform("ml_matrix", mat4::Translate(vec3(-4, 0, 0)) * transform * mat4::Scale(vec3(1.4f, 1.4f, 1.4f)));
	if (m_SetUniforms[1])
		m_SphereMaterial->SetUniform("ml_matrix", mat4::Translate(vec3(4, 0, 0)) * transform);

	if (m_Window->IsKeyTyped(VK_1))
	{
		m_CubeMaterial->UnsetUniform("ml_matrix");
		m_SetUniforms[0] = !m_SetUniforms[0];
	}
	if (m_Window->IsKeyTyped(VK_2))
	{
		m_SphereMaterial->UnsetUniform("ml_matrix");
		m_SetUniforms[1] = !m_SetUniforms[1];
	}

}

bool Test3D::OnEvent(const sp::events::Event& event)
{
	return false;
}

