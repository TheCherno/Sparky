#include "ScriptingTest.h"

#include <sp\scripting\Scripting.h>

using namespace sp;
using namespace debug;
using namespace graphics;
using namespace maths;

using namespace events;
using namespace entity;
using namespace component;

using namespace API;

using namespace scripting;

ScriptingTest::ScriptingTest()
	: Layer3D(spnew Scene())
{
	m_MayaCamera = m_Scene->GetCamera();
	m_FPSCamera = spnew FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));

	m_Rotation = 0.0f;
	m_SetUniforms[0] = true;
	m_SetUniforms[1] = true;
}

ScriptingTest::~ScriptingTest()
{
}

Mesh* g_Mesh;

String materialInput[5] =
{
	"CastIron",
	"WornWood",
	"GunMetal",
	"ABSRed",
	"Custom"
};

lua_State* m_ScriptingState;

void ScriptingTest::OnInit(Renderer3D& renderer, Scene& scene)
{
	// Enable this to use FPS camera
	// scene.SetCamera(m_FPSCamera);

	Timer timer;
	String environmentFiles[11] =
	{
		"/pbr/cubemap/CubeMap0.tga",
		"/pbr/cubemap/CubeMap1.tga",
		"/pbr/cubemap/CubeMap2.tga",
		"/pbr/cubemap/CubeMap3.tga",
		"/pbr/cubemap/CubeMap4.tga",
		"/pbr/cubemap/CubeMap5.tga",
		"/pbr/cubemap/CubeMap6.tga",
		"/pbr/cubemap/CubeMap7.tga",
		"/pbr/cubemap/CubeMap8.tga",
		"/pbr/cubemap/CubeMap9.tga",
		"/pbr/cubemap/CubeMap10.tga"
	};

	TextureCube* environment = TextureCube::CreateFromVCross(environmentFiles, 11);
	Shader* skybox = Shader::CreateFromFile("Skybox", String("/shaders/Skybox") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	Material* skyboxMaterial = spnew Material(skybox);
	skyboxMaterial->SetRenderFlag(Material::RenderFlags::DISABLE_DEPTH_TEST);
	skybox->Bind();
	m_SkyboxMaterial = spnew MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = spnew Entity(MeshFactory::CreateQuad(-1, -1, 2, 2, m_SkyboxMaterial));
	m_Scene->Add(skyboxEntity);

	Shader* pbrShader = Shader::CreateFromFile("AdvancedLighting", String("/shaders/AdvancedLighting") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	ShaderManager::Add(pbrShader);

	PBRMaterial* castIron = spnew PBRMaterial(pbrShader);
	castIron->SetEnviromentMap(environment);
	{
		String path = materialInput[CAST_IRON] + "/" + materialInput[CAST_IRON];
		castIron->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		castIron->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		castIron->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		castIron->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(castIron);

	Model* sphereModel = spnew Model("/models/Sphere.spm");

	int x = 0;
	float xx = x * 10.0f;
	float zz = 0;

	float roughness = x / 10.0f;
	vec3 spec(1.0f);
	vec4 diffuse(0.0f, 0.0f, 0.0f, 1.0f);

	PBRMaterialInstance* m = spnew PBRMaterialInstance(m_Materials[CAST_IRON]);
	m->SetAlbedo(diffuse);
	m->SetSpecular(spec);
	m->SetGloss(1.0f - roughness);
	m->UsingNormalMap(false);

	g_Mesh = spnew Mesh(sphereModel->GetMesh());
	g_Mesh->SetMaterial(m);

	Scripting::CreateState(&m_ScriptingState);
	Scripting::Init(m_ScriptingState);
	Scripting::LoadSparkyAPI(m_ScriptingState);
	Scripting::LoadFile(m_ScriptingState, "/scripts/test.lua");

	Scripting::Call(m_ScriptingState, "printVersion");

	Entity* e = spnew Entity();
	e->AddComponent(spnew MeshComponent(g_Mesh));
	e->AddComponent(spnew TransformComponent(mat4::Identity() * mat4::Translate(vec3(20, 0, 20))));
	m_Scene->Add(e);

	SP_INFO("Init took ", timer.ElapsedMillis(), " ms");
}

void ScriptingTest::OnTick()
{
	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
}

void ScriptingTest::OnUpdate()
{

}

void ScriptingTest::OnRender(Renderer3D& renderer)
{
	Layer3D::OnRender(renderer);
}

void ScriptingTest::OnEvent(Event& event)
{
	if (event.GetType() == Event::Type::KEY_PRESSED)
	{
		KeyPressedEvent* kpe = (KeyPressedEvent*)&event;
		if (kpe->GetRepeat() == 0)
		{
			switch (kpe->GetKeyCode())
			{
			case SP_KEY_R:
				ShaderManager::Reload("AdvancedLighting");
				break;
			case SP_KEY_C:
				m_Scene->SetCamera(m_Scene->GetCamera() == m_MayaCamera ? m_FPSCamera : m_MayaCamera);
				break;
			case SP_KEY_O:
				Scripting::Call(m_ScriptingState, "loadSound", "cherno", "res/Cherno.ogg");
				break;
			case SP_KEY_P:
				Scripting::Call(m_ScriptingState, "playSound", "cherno");
				break;
			case SP_KEY_L:
				Scripting::Call(m_ScriptingState, "loopSound", "cherno");
				break;
			case SP_KEY_H:
				Scripting::Call(m_ScriptingState, "debugMenu", "yes", false);
				break;
			case SP_KEY_E:
				Scripting::Call(m_ScriptingState, "addEntity", maths::vec3(0, 20, 0), g_Mesh, m_Scene);
				break;
			}
		}
		switch (kpe->GetKeyCode())
		{
		case SP_KEY_1:
			Scripting::Call(m_ScriptingState, "changeGain", -0.4);
			break;
		case SP_KEY_2:
			Scripting::Call(m_ScriptingState, "changeGain", 0.4);
			break;
		}
	}
	Layer::OnEvent(event);
}
