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
	m_MayaCameras = m_Scene->GetCamera();
	m_FPSCameras = spnew FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));

	m_Rotations = 0.0f;
	m_SetUniformss[0] = true;
	m_SetUniformss[1] = true;

	mat4 result = mat4::Identity() * mat4::Identity();
}

ScriptingTest::~ScriptingTest()
{
}

vec3 g_CubeTransforms(-10, 10, 0);
vec3 g_DaggerTransforms(0, 20, 0);
vec4 g_SphereColors(0.0f, 0.0f, 0.0f, 1.0f);
vec3 g_SphereSpecularColors(1.0f, 1.0f, 0.6f);

float g_DaggerGlosss = 0.5f;

Shader* shadowPassShaders;
FramebufferDepth* g_DepthBuffers;
TextureDepth* g_ShadowMaps;

String materialInputss[5] =
{
	"CastIron",
	"WornWood",
	"GunMetal",
	"ABSRed",
	"Custom"
};

enum Materials : uint
{
	CAST_IRON = 0,
	WORN_WOOD,
	GUN_METAL,
	ABS_RED,
	CUSTOM
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
	m_SkyboxMaterials = spnew MaterialInstance(skyboxMaterial);
	m_SkyboxMaterials->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = spnew Entity(MeshFactory::CreateQuad(-1, -1, 2, 2, m_SkyboxMaterials));
	m_Scene->Add(skyboxEntity);

	Shader* pbrShader = Shader::CreateFromFile("AdvancedLighting", String("/shaders/AdvancedLighting") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	ShaderManager::Add(pbrShader);
	PBRMaterial* material = spnew PBRMaterial(pbrShader);

	PBRMaterial* castIron = spnew PBRMaterial(pbrShader);
	castIron->SetEnviromentMap(environment);
	{
		String path = materialInputss[CAST_IRON] + "/" + materialInputss[CAST_IRON];
		castIron->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		castIron->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		castIron->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		castIron->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materialss.push_back(castIron);

	PBRMaterial* wornWood = spnew PBRMaterial(pbrShader);
	wornWood->SetEnviromentMap(environment);
	{
		String path = materialInputss[WORN_WOOD] + "/" + materialInputss[WORN_WOOD];
		wornWood->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		wornWood->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		wornWood->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		wornWood->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materialss.push_back(wornWood);

	PBRMaterial* gunMetal = spnew PBRMaterial(pbrShader);
	gunMetal->SetEnviromentMap(environment);
	{
		String path = materialInputss[GUN_METAL] + "/" + materialInputss[GUN_METAL];
		gunMetal->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		gunMetal->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		gunMetal->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		gunMetal->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materialss.push_back(gunMetal);


	PBRMaterial* absRed = spnew PBRMaterial(pbrShader);
	absRed->SetEnviromentMap(environment);
	{
		String path = materialInputss[ABS_RED] + "/" + materialInputss[ABS_RED];
		absRed->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		absRed->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		absRed->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		absRed->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materialss.push_back(absRed);

	PBRMaterial* custom = spnew PBRMaterial(pbrShader);
	custom->SetEnviromentMap(environment);
	{
		String path = materialInputss[CUSTOM] + "/" + materialInputss[CUSTOM];
		custom->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		custom->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		custom->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		custom->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materialss.push_back(custom);

	// Texture::SetLoadParameters(0);
	m_DaggerMaterials = spnew PBRMaterial(pbrShader);
	m_DaggerMaterials->SetEnviromentMap(environment);
	{
		TextureLoadOptions options(false, true);
		m_DaggerMaterials->SetAlbedoMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Albedo.tga", options));
		m_DaggerMaterials->SetSpecularMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Specular.tga", options));
		m_DaggerMaterials->SetGlossMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Gloss.tga", options));
		m_DaggerMaterials->SetNormalMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Normals.tga", options));
	}

	Model* daggerModel = spnew Model("/models/Dagger.spm", spnew MaterialInstance(m_DaggerMaterials));
	m_Daggers = spnew Entity(daggerModel->GetMesh(), mat4::Translate(g_DaggerTransforms));
	m_Scene->Add(m_Daggers);

	PBRMaterial* cubeMaterial = spnew PBRMaterial(pbrShader);
	cubeMaterial->SetEnviromentMap(environment);
	Model* cubeModel = spnew Model("/models/RoundedCube.spm", spnew MaterialInstance(cubeMaterial));
	m_Cubes = spnew Entity(cubeModel->GetMesh(), mat4::Rotate(90.0f, vec3(0, 0, 1)) * mat4::Translate(g_CubeTransforms));
	m_Scene->Add(m_Cubes);

	Model* sphereModel = spnew Model("/models/Sphere.spm");

	// Plastics
	for (uint x = 0; x < 10; x++)
	{
		float xx = x * 10.0f;
		float zz = 3.0f;

		float roughness = x / 10.0f;
		vec3 spec(0.04f);
		vec4 diffuse(vec3(1.0f, 0.0f, 0.0f), 1.0f);

		PBRMaterialInstance* m = spnew PBRMaterialInstance(m_Materialss[CAST_IRON]);
		m->SetAlbedo(diffuse);
		m->SetSpecular(spec);
		m->SetGloss(1.0f - roughness);
		m->UsingNormalMap(false);

		Mesh* mesh = spnew Mesh(sphereModel->GetMesh());
		mesh->SetMaterial(m);

		Entity* sphere = spnew Entity(mesh, mat4::Translate(vec3(-60 + xx, 2.5f, 90 + zz)) * mat4::Scale(vec3(2.0f)));
		m_Spheress.push_back(sphere);
		m_Scene->Add(sphere);
	}

	// Metals
	for (uint x = 0; x < 10; x++)
	{
		float xx = x * 10.0f;
		float zz = 0;

		float roughness = x / 10.0f;
		vec3 spec(1.0f);
		vec4 diffuse(0.0f, 0.0f, 0.0f, 1.0f);

		PBRMaterialInstance* m = spnew PBRMaterialInstance(m_Materialss[CAST_IRON]);
		m->SetAlbedo(diffuse);
		m->SetSpecular(spec);
		m->SetGloss(1.0f - roughness);
		m->UsingNormalMap(false);

		Mesh* mesh = spnew Mesh(sphereModel->GetMesh());
		mesh->SetMaterial(m);

		Entity* sphere = spnew Entity(mesh, mat4::Translate(vec3(-60 + xx, 2.5f, 80 + zz)) * mat4::Scale(vec3(2.0)));
		m_Spheress.push_back(sphere);
		m_Scene->Add(sphere);
	}

	m_Planes = spnew Entity(MeshFactory::CreatePlane(128, 128, vec3(0, 1, 0), spnew PBRMaterialInstance(m_Materialss[CUSTOM])));
	m_Scene->Add(m_Planes);

	LightSetup* lights = spnew LightSetup();
	m_Lights = spnew Light(vec3(0.8f));
	lights->Add(m_Lights);
	m_Scene->PushLightSetup(lights);

	DebugMenu::Add("Cube", &g_CubeTransforms, -100.0f, 100.0f);
	DebugMenu::Add("Light Direction", &lights->GetLights()[0]->direction, -1.0f, 1.0f);
	DebugMenu::Add("Light Intensity", &lights->GetLights()[0]->intensity, 0, 100);
	DebugMenu::Add("Dagger", &g_DaggerTransforms, -50, 50);
	DebugMenu::Add("Dagger Gloss", &g_DaggerGlosss, 0.0f, 1.0f);

	// shadowPassShader = Shader::CreateFromFile("Shadow", "shaders/ShadowPass.shader");
	// g_DepthBuffer = FramebufferDepth::Create(2048, 2048);
	// g_ShadowMap = (TextureDepth*)g_DepthBuffer->GetTexture();

	for (uint i = 0; i < 4; i++)
	{
		// m_Materials[i]->SetTexture(g_ShadowMap, 6);
		// m_Materials[i]->SetUniform("u_ShadowMap", 6);
	}

	Scripting::CreateState(&m_ScriptingState);
	Scripting::Init(m_ScriptingState);
	Scripting::LoadSparkyAPI(m_ScriptingState);
	Scripting::LoadFile(m_ScriptingState, "/scripts/test.lua");

	Scripting::Call(m_ScriptingState, "printVersion");
	Scripting::Call(m_ScriptingState, "playSound");

	SP_INFO("Init took ", timer.ElapsedMillis(), " ms");
}

void ScriptingTest::OnTick()
{
	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
}

void ScriptingTest::OnUpdate()
{
	TransformComponent* cubeTransform = m_Cubes->GetComponent<TransformComponent>();

	mat4 transform = mat4::Translate(vec3(0, 2.5f, 0)) * mat4::Rotate(m_Rotations, vec3(1, 0, 0)) * mat4::Rotate(m_Rotations, vec3(0, 1, 0)) * mat4::Rotate(m_Rotations, vec3(0, 0, 1));
	cubeTransform->transform = mat4::Translate(g_CubeTransforms) * transform * mat4::Scale(vec3(1.4f, 1.4f, 1.4f));

	TransformComponent* dagger = m_Daggers->GetComponent<TransformComponent>();
	dagger->transform = mat4::Translate(g_DaggerTransforms);

	for (Entity* sphere : m_Spheress)
	{
		TransformComponent* sphereTransform = sphere->GetComponent<TransformComponent>();
		//	sphereTransform->transform = sphereTransform->transform * mat4::Rotate(1.0f, vec3(0, 1, 0));
	}

	// Still OpenGL maths style (column-major)
	mat4 vp = m_Scene->GetCamera()->GetProjectionMatrix() * m_Scene->GetCamera()->GetViewMatrix();
	m_SkyboxMaterials->SetUniform("invViewProjMatrix", mat4::Invert(vp));


}

void ScriptingTest::OnRender(Renderer3D& renderer)
{
#if 0
	mat4 depthProjectionMatrix = mat4::Orthographic(-10, 10, -10, 10, -10, 20);
	mat4 depthViewMatrix = mat4::LookAt(vec3(0.5f, 0.5f, 2.0f), vec3(), vec3::Up());
	mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

	g_DepthBuffer->Bind();
	g_DepthBuffer->Clear();
	renderer.Begin();
	for (uint i = 0; i < m_Scene->GetEntities().size(); i++)
	{
		Entity* e = m_Scene->GetEntities()[i];
		RenderCommand command;
		command.mesh = e->GetComponent<MeshComponent>()->mesh;
		command.shader = shadowPassShader;
		command.uniforms.push_back({ "ml_matrix", (byte*)&e->GetComponent<TransformComponent>()->transform });
		command.uniforms.push_back({ "u_DepthMatrix", (byte*)&depthMVP });
		renderer.Submit(command);
	}
	renderer.End();
	renderer.Present();

	g_DepthBuffer->Unbind();

	float elements[4 * 4] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};
	mat4 biasMatrix(elements);
	mat4 depthBiasMVP = biasMatrix * depthMVP;

	for (Material* material : m_Materials)
		material->SetUniform("u_DepthBiasMVP", depthBiasMVP);

#endif

	//DebugLayer::DrawTexture(m_DaggerMaterial->GetGlossMap());
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
				m_Scene->SetCamera(m_Scene->GetCamera() == m_MayaCameras ? m_FPSCameras : m_MayaCameras);
				break;
			}
		}
	}
	Layer::OnEvent(event);
}
