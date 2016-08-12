#include "DeferredTest.h"

using namespace sp;
using namespace debug;
using namespace graphics;
using namespace maths;

using namespace entity;
using namespace component;

using namespace API;

#if 0

DeferredTest::DeferredTest()
	: Layer3D(new Scene())
{
	m_Rotation = 0.0f;
	m_SetUniforms[0] = true;
	m_SetUniforms[1] = true;
}

DeferredTest::~DeferredTest()
{
}

static vec3 g_CubeTransform(-4, 0, 0);
static vec3 g_LightDir(0.5f, 2, 2);
static float roughness = 0.5f;
static Material* material;

static Scene* g_Scene;

void DeferredTest::OnInit(Renderer3D& renderer, Scene& scene)
{
	g_Scene = &scene;

	Texture* crate = TextureManager::Add(Texture2D::CreateFromFile("Crate", "res/Crate.jpg"));

	m_Scene = &scene;
	material = new Material(Shader::CreateFromFile("AdvancedLighting", "shaders/AdvancedLighting.shader"));

	MaterialInstance* cubeMaterial = new MaterialInstance(material);
	cubeMaterial->SetUniform("u_Roughness", 0.5f);

	m_Cube = new Entity();
	Model* cubeModel = new Model("res/Cube.obj", cubeMaterial);
	m_Cube->AddComponent(new MeshComponent(cubeModel->GetMesh()));
	m_Cube->AddComponent(new TransformComponent(mat4::Identity()));

	m_Plane = new Entity();
	m_Plane->AddComponent(new MeshComponent(MeshFactory::CreatePlane(128, 128, vec3(0, 1, 0), new MaterialInstance(material))));
	m_Plane->AddComponent(new TransformComponent(mat4::Identity()));

	m_Scene->Add(m_Cube);
	m_Scene->Add(m_Plane);

	Entity* obj = new Entity();
	Model* objModel = new Model("res/Plane.obj", new MaterialInstance(material));
	obj->AddComponent(new MeshComponent(objModel->GetMesh()));
	obj->AddComponent(new TransformComponent(mat4::Identity()));
	m_Scene->Add(obj);

	LightSetup* lights = new LightSetup();
	lights->Add(new Light(vec3(0.8f)));
	m_Scene->PushLightSetup(lights);

	DebugMenu::Add("Cube", &g_CubeTransform, -20.0f, 20.0f);
	DebugMenu::Add("LightDir", &g_LightDir, -20.0f, 20.0f);
	DebugMenu::Add("Light Pos", &lights->GetLights()[0]->position, -20.0f, 40.0f);
	DebugMenu::Add("Light Atten.", &lights->GetLights()[0]->intensity, 0, 100);
	DebugMenu::Add("Roughness", &roughness, 0, 1);

	DeferredRenderer& r = (DeferredRenderer&)renderer;
//	DebugLayer::GetInstance()->Add(new Sprite(0, 0, 8, 4.5f, new Texture2D(r.GetGBuffer().GetTexture(0))));
}

void DeferredTest::OnTick()
{
	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
}

void DeferredTest::OnUpdate(const Timestep& ts)
{
	material->SetUniform("u_CameraPos", m_Scene->GetCamera()->GetPosition());

	TransformComponent* cubeTransform = m_Cube->GetComponent<TransformComponent>();
	// TransformComponent* sphereTransform = m_Sphere->GetComponent<TransformComponent>();

	mat4 transform = mat4::Translate(vec3(0, 2.5f, 0)) * mat4::Rotate(m_Rotation, vec3(1, 0, 0)) * mat4::Rotate(m_Rotation, vec3(0, 1, 0)) * mat4::Rotate(m_Rotation, vec3(0, 0, 1));
	cubeTransform->transform = mat4::Translate(g_CubeTransform) * transform * mat4::Scale(vec3(1.4f, 1.4f, 1.4f));
	// sphereTransform->transform = mat4::Translate(vec3(4, 0, 0)) * transform;
	//m_Rotation++;

	material->SetUniform("u_Roughness", roughness);
}

void DeferredTest::OnRender(Renderer3D& renderer)
{
	Layer3D::OnRender(renderer);
}

void DeferredTest::OnEvent(sp::events::Event& event)
{
}

#endif