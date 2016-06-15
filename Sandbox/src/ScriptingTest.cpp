#include "ScriptingTest.h"

#include <sp\scripting\spslua.h>
#include <sp\system\Reference.h>

using namespace sp;
using namespace debug;
using namespace graphics;
using namespace maths;

using namespace events;
using namespace entity;
using namespace component;

using namespace API;

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

	SP_NEWSTATE();
	SP_INIT();
	SP_LOADAPI();
	SP_LOADFILE("res/scripts/test.lua");

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

sp::audio::Sound* s = nullptr;

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
				SP_CALLFUNCTION("loadSound", "cherno", "res/Cherno.ogg");
				s = audio::SoundManager::Get("cherno");
				break;
			case SP_KEY_P:
				SP_CALLFUNCTION("playSound", s);
				break;
			case SP_KEY_L:
				SP_CALLFUNCTION("loopSound", s);
				break;
			case SP_KEY_H:
				SP_CALLFUNCTION("debugMenu", "yes", false);
				break;
			}
		}
		switch (kpe->GetKeyCode())
		{
		case SP_KEY_1:
			SP_CALLFUNCTION("changeGain", s, -0.1f);
			break;
		case SP_KEY_2:
			SP_CALLFUNCTION("changeGain", s, 0.1f);
			break;
		}
	}
	Layer::OnEvent(event);
}
