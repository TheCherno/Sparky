#include "ShadowTest.h"

using namespace sp;
using namespace debug;
using namespace graphics;
using namespace maths;

using namespace events;
using namespace entity;
using namespace component;

using namespace API;

ShadowTest::ShadowTest()
	: Layer3D(spnew Scene()), m_CubePosition(0, 5, 0), m_ShowShadowMap(true)
{
	m_MayaCamera = m_Scene->GetCamera();
	m_FPSCamera = spnew FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
}

ShadowTest::~ShadowTest()
{

}

void ShadowTest::OnInit(Renderer3D& renderer, Scene& scene)
{
	String environmentFiles[11] =
	{
		"res/pbr/cubemap/CubeMap0.tga",
		"res/pbr/cubemap/CubeMap1.tga",
		"res/pbr/cubemap/CubeMap2.tga",
		"res/pbr/cubemap/CubeMap3.tga",
		"res/pbr/cubemap/CubeMap4.tga",
		"res/pbr/cubemap/CubeMap5.tga",
		"res/pbr/cubemap/CubeMap6.tga",
		"res/pbr/cubemap/CubeMap7.tga",
		"res/pbr/cubemap/CubeMap8.tga",
		"res/pbr/cubemap/CubeMap9.tga",
		"res/pbr/cubemap/CubeMap10.tga"
	};
	TextureCube* environment = TextureCube::CreateFromVCross(environmentFiles, 11);
	
	Shader* skybox = Shader::CreateFromFile("Skybox", String("shaders/Skybox") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	Material* skyboxMaterial = spnew Material(skybox);
	skyboxMaterial->SetRenderFlag(Material::RenderFlags::DISABLE_DEPTH_TEST);
	skybox->Bind();
	m_SkyboxMaterial = spnew MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = spnew Entity(MeshFactory::CreateQuad(-1, -1, 2, 2, m_SkyboxMaterial));
	m_Scene->Add(skyboxEntity);

	Shader* pbrShader = Shader::CreateFromFile("AdvancedLighting", String("shaders/AdvancedLighting") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	ShaderManager::Add(pbrShader);

	PBRMaterial* material = spnew PBRMaterial(pbrShader);
	material->SetEnviromentMap(environment);
	material->SetGloss(0.2f);

	auto cubeMaterial = spnew PBRMaterialInstance(material);
	cubeMaterial->SetAlbedo(vec4(0.2f, 0.3f, 0.8f, 1.0f));
	cubeMaterial->SetSpecular(vec3(0.2f, 0.3f, 0.8f));
	Model* cubeModel = spnew Model("res/models/RoundedCube.spm", cubeMaterial);
	m_Cube = spnew Entity(cubeModel->GetMesh(), mat4::Translate(vec3(5, 0, 0)) * mat4::Rotate(90.0f, vec3(0, 0, 1)));
	m_Scene->Add(m_Cube);

	Entity* plane = spnew Entity(MeshFactory::CreatePlane(128, 128, vec3(0, 1, 0), spnew PBRMaterialInstance(material)));
	m_Scene->Add(plane);

	LightSetup* lights = spnew LightSetup();
	Light* light = spnew Light(vec3(0.8f));
	light->position = vec3(2, 2, 2);
	lights->Add(light);
	m_Scene->PushLightSetup(lights);

	DebugMenu::Add("Light Dir", &light->direction, -1.0f, 1.0f);
	DebugMenu::Add("Light Pos", &light->position, -20.0f, 20.0f);
	DebugMenu::Add("Show Shadow Map", &m_ShowShadowMap);
	DebugMenu::Add("Cube", &m_CubePosition, -100.0f, 100.0f);
}

void ShadowTest::OnTick()
{
}

void ShadowTest::OnUpdate()
{
	// Still OpenGL maths style (column-major)
	mat4 vp = m_Scene->GetCamera()->GetViewMatrix() * m_Scene->GetCamera()->GetProjectionMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));

	TransformComponent* cubeTransform = m_Cube->GetComponent<TransformComponent>();
	cubeTransform->transform = mat4::Translate(m_CubePosition);
}

void ShadowTest::OnEvent(sp::events::Event& event)
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

void ShadowTest::OnRender(Renderer3D& renderer)
{
	if (m_ShowShadowMap)
		DebugLayer::DrawTexture(((ForwardRenderer&)renderer).GetDepthTexture(), vec2(24.0f, 0.0f));
	Layer3D::OnRender(renderer);
}
