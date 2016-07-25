#include "VoxelTest.h"

using namespace sp;
using namespace debug;
using namespace graphics;
using namespace maths;

using namespace events;
using namespace entity;
using namespace component;

using namespace API;

VoxelTest::VoxelTest()
	: Layer3D(spnew Scene())
{
	m_MayaCamera = m_Scene->GetCamera();
	m_FPSCamera = spnew FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));

	m_Rotation = 0.0f;
	m_SetUniforms[0] = true;
	m_SetUniforms[1] = true;

	mat4 result = mat4::Identity() * mat4::Identity();
}

VoxelTest::~VoxelTest()
{
}

static vec3 g_CubeTransform(-10, 10, 0);
static vec3 g_DaggerTransform(0, 20, 0);
static vec4 g_SphereColor(0.0f, 0.0f, 0.0f, 1.0f);
static vec3 g_SphereSpecularColor(1.0f, 1.0f, 0.6f);

static float g_DaggerGloss = 0.5f;

static String materialInputs[5] =
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

void VoxelTest::OnInit(Renderer3D& renderer, Scene& scene)
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
	PBRMaterial* material = spnew PBRMaterial(pbrShader);

	PBRMaterial* castIron = spnew PBRMaterial(pbrShader);
	castIron->SetEnviromentMap(environment);
	{
		String path = materialInputs[CAST_IRON] + "/" + materialInputs[CAST_IRON];
		castIron->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		castIron->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		castIron->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		castIron->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(castIron);

	PBRMaterial* wornWood = spnew PBRMaterial(pbrShader);
	wornWood->SetEnviromentMap(environment);
	{
		String path = materialInputs[WORN_WOOD] + "/" + materialInputs[WORN_WOOD];
		wornWood->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		wornWood->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		wornWood->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		wornWood->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(wornWood);

	PBRMaterial* gunMetal = spnew PBRMaterial(pbrShader);
	gunMetal->SetEnviromentMap(environment);
	{
		String path = materialInputs[GUN_METAL] + "/" + materialInputs[GUN_METAL];
		gunMetal->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		gunMetal->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		gunMetal->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		gunMetal->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(gunMetal);


	PBRMaterial* absRed = spnew PBRMaterial(pbrShader);
	absRed->SetEnviromentMap(environment);
	{
		String path = materialInputs[ABS_RED] + "/" + materialInputs[ABS_RED];
		absRed->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		absRed->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		absRed->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		absRed->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(absRed);

	PBRMaterial* custom = spnew PBRMaterial(pbrShader);
	custom->SetEnviromentMap(environment);
	{
		String path = materialInputs[CUSTOM] + "/" + materialInputs[CUSTOM];
		custom->SetAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		custom->SetSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		custom->SetGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		custom->SetNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(custom);

	// Texture::SetLoadParameters(0);
	m_DaggerMaterial = spnew PBRMaterial(pbrShader);
	m_DaggerMaterial->SetEnviromentMap(environment);
	{
		TextureLoadOptions options(false, true);
		m_DaggerMaterial->SetAlbedoMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Albedo.tga", options));
		m_DaggerMaterial->SetSpecularMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Specular.tga", options));
		m_DaggerMaterial->SetGlossMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Gloss.tga", options));
		m_DaggerMaterial->SetNormalMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Normals.tga", options));
	}

	Model* daggerModel = spnew Model("/models/Dagger.spm", spnew MaterialInstance(m_DaggerMaterial));
	m_Dagger = spnew Entity(daggerModel->GetMesh(), mat4::Translate(g_DaggerTransform));
	m_Scene->Add(m_Dagger);

	PBRMaterial* cubeMaterial = spnew PBRMaterial(pbrShader);
	cubeMaterial->SetEnviromentMap(environment);
	Model* cubeModel = spnew Model("/models/RoundedCube.spm", spnew MaterialInstance(cubeMaterial));
	m_Cube = spnew Entity(cubeModel->GetMesh(), mat4::Rotate(90.0f, vec3(0, 0, 1)) * mat4::Translate(g_CubeTransform));
	m_Scene->Add(m_Cube);

	Model* sphereModel = spnew Model("/models/Sphere.spm");

	// Plastics
	for (uint x = 0; x < 10; x++)
	{
		float xx = x * 10.0f;
		float zz = 3.0f;

		float roughness = x / 10.0f;
		vec3 spec(0.04f);
		vec4 diffuse(vec3(1.0f, 0.0f, 0.0f), 1.0f);

		PBRMaterialInstance* m = spnew PBRMaterialInstance(m_Materials[CAST_IRON]);
		m->SetAlbedo(diffuse);
		m->SetSpecular(spec);
		m->SetGloss(1.0f - roughness);
		m->UsingNormalMap(false);

		Mesh* mesh = spnew Mesh(sphereModel->GetMesh());
		mesh->SetMaterial(m);

		Entity* sphere = spnew Entity(mesh, mat4::Translate(vec3(-60 + xx, 2.5f, 90 + zz)) * mat4::Scale(vec3(2.0f)));
		m_Spheres.push_back(sphere);
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

		PBRMaterialInstance* m = spnew PBRMaterialInstance(m_Materials[CAST_IRON]);
		m->SetAlbedo(diffuse);
		m->SetSpecular(spec);
		m->SetGloss(1.0f - roughness);
		m->UsingNormalMap(false);

		Mesh* mesh = spnew Mesh(sphereModel->GetMesh());
		mesh->SetMaterial(m);

		Entity* sphere = spnew Entity(mesh, mat4::Translate(vec3(-60 + xx, 2.5f, 80 + zz)) * mat4::Scale(vec3(2.0)));
		m_Spheres.push_back(sphere);
		m_Scene->Add(sphere);
	}

	m_Plane = spnew Entity(MeshFactory::CreatePlane(128, 128, vec3(0, 1, 0), spnew PBRMaterialInstance(m_Materials[CUSTOM])));
	m_Scene->Add(m_Plane);

	LightSetup* lights = spnew LightSetup();
	m_Light = spnew Light(vec3(0.8f));
	lights->Add(m_Light);
	m_Scene->PushLightSetup(lights);

	DebugMenu::Add("Cube", &g_CubeTransform, -100.0f, 100.0f);
	DebugMenu::Add("Light Direction", &lights->GetLights()[0]->direction, -1.0f, 1.0f);
	DebugMenu::Add("Light Intensity", &lights->GetLights()[0]->intensity, 0, 100);
	DebugMenu::Add("Dagger", &g_DaggerTransform, -50, 50);
	DebugMenu::Add("Dagger Gloss", &g_DaggerGloss, 0.0f, 1.0f);

	SP_INFO("Init took ", timer.ElapsedMillis(), " ms");
}

void VoxelTest::OnTick()
{
	Application& app = Application::GetApplication();
	SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
}

void VoxelTest::OnUpdate(const Timestep& ts)
{
 	TransformComponent* cubeTransform = m_Cube->GetComponent<TransformComponent>();
 
 	mat4 transform = mat4::Translate(vec3(0, 2.5f, 0)) * mat4::Rotate(m_Rotation, vec3(1, 0, 0)) * mat4::Rotate(m_Rotation, vec3(0, 1, 0)) * mat4::Rotate(m_Rotation, vec3(0, 0, 1));
 	cubeTransform->transform = mat4::Translate(g_CubeTransform) * transform * mat4::Scale(vec3(1.4f, 1.4f, 1.4f));

	TransformComponent* dagger = m_Dagger->GetComponent<TransformComponent>();
	dagger->transform = mat4::Translate(g_DaggerTransform);

	// Still OpenGL maths style (column-major)
	mat4 vp = m_Scene->GetCamera()->GetProjectionMatrix() * m_Scene->GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));
}

void VoxelTest::OnRender(Renderer3D& renderer)
{
	Layer3D::OnRender(renderer);
}

void VoxelTest::OnEvent(Event& event)
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
			}
		}
	}
	Layer::OnEvent(event);
}
