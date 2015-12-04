#include "Test3D.h"

using namespace sp;
using namespace graphics;
using namespace maths;

Test3D::Test3D()
	: Layer3D(new Scene())
{
	m_Rotation = 0.0f;
}

Test3D::~Test3D()
{
}

void Test3D::OnInit(Renderer3D& renderer, Scene& scene)
{
	VertexArray* va = MeshFactory::CreateQuad(0, 0, 5, 5);
	IndexBuffer* ib = new IndexBuffer(new uint[6]{ 0, 1, 2, 2, 3, 0 }, 6);

	m_Material = new Material(Shader::FromFile("Scene", "shaders/Scene.shader"));
	m_Cube = MeshFactory::CreateCube(5.0f, new MaterialInstance(m_Material));
	scene.Add(m_Cube);

	m_Material->SetUniform("pr_matrix", mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
	m_Material->SetUniform("vw_matrix", mat4::Translate(vec3(0, 0, -10.0f)));
	m_Material->SetUniform("ml_matrix", mat4::Rotate(45.0f, vec3(0, 1, 0)));
}

void Test3D::OnTick()
{
	Application& app = Application::GetApplication();
	SPARKY_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");

	// m_Cube->GetMaterialInstance()->GetMaterial()->DumpUniformData();
}

float g_Rotation = 0.0f;

void Test3D::OnUpdate()
{
	mat4 transform = mat4::Rotate(g_Rotation, vec3(1, 0, 0)) * mat4::Rotate(g_Rotation, vec3(0, 1, 0)) * mat4::Rotate(g_Rotation, vec3(0, 0, 1));
	m_Material->SetUniform("ml_matrix", transform);
	g_Rotation += 0.5f;
}

bool Test3D::OnEvent(const sp::events::Event& event)
{
	return false;
}

