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
}

VoxelTest::~VoxelTest()
{
}

static vec3 g_CubeTransform(-10, 10, 0);
static vec4 g_SphereColor(0.0f, 0.0f, 0.0f, 1.0f);
static vec3 g_SphereSpecularColor(1.0f, 1.0f, 0.6f);

static String materialInputs[2] =
{
	"CastIron",
	"Custom"
};

enum Materials : uint
{
	CAST_IRON = 0,
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
		mesh->SetDebugDraw(true);

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
 	cubeTransform->transform = mat4::Translate(g_CubeTransform) * mat4::Scale(vec3(1.4f, 1.4f, 1.4f));

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
