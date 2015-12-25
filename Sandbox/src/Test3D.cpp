#include "Test3D.h"

using namespace sp;
using namespace graphics;
using namespace maths;

using namespace entity;
using namespace component;

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
	Material* material = new Material(Shader::FromFile("Scene", "shaders/Scene.shader"));

	m_Cube = new Entity();
	Model* cubeModel = new Model("res/Cube.obj", new MaterialInstance(material));
	m_Cube->AddComponent(new MeshComponent(cubeModel->GetMesh()));
	m_Cube->AddComponent(new TransformComponent(mat4::Identity()));

	m_Sphere = new Entity();
	Model* sphereModel = new Model("res/Sphere.obj", new MaterialInstance(material));
	m_Sphere->AddComponent(new MeshComponent(sphereModel->GetMesh()));
	m_Sphere->AddComponent(new TransformComponent(mat4::Identity()));

	m_Plane = new Entity();
	m_Plane->AddComponent(new MeshComponent(MeshFactory::CreatePlane(32, 32, vec3(0, 1, 0), new MaterialInstance(material))));
	m_Plane->AddComponent(new TransformComponent(mat4::Identity()));

	m_Scene->Add(m_Cube);
	m_Scene->Add(m_Sphere);
	m_Scene->Add(m_Plane);
}

void Test3D::OnTick()
{
	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
}

void Test3D::OnUpdate()
{
	TransformComponent* cubeTransform = m_Cube->GetComponent<TransformComponent>();
	TransformComponent* sphereTransform = m_Sphere->GetComponent<TransformComponent>();

	mat4 transform = mat4::Translate(vec3(0, 2.5f, 0)) * mat4::Rotate(m_Rotation, vec3(1, 0, 0)) * mat4::Rotate(m_Rotation, vec3(0, 1, 0)) * mat4::Rotate(m_Rotation, vec3(0, 0, 1));
	cubeTransform->transform = mat4::Translate(vec3(-4, 0, 0)) * transform * mat4::Scale(vec3(1.4f, 1.4f, 1.4f));
	sphereTransform->transform = mat4::Translate(vec3(4, 0, 0)) * transform;
	m_Rotation++;
}

bool Test3D::OnEvent(const sp::events::Event& event)
{
	return false;
}

