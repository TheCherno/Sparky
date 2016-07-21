#include "SponzaTest.h"

using namespace sp;
using namespace debug;
using namespace graphics;
using namespace maths;

using namespace events;
using namespace entity;
using namespace component;

using namespace API;

SponzaTest::SponzaTest()
	: Layer3D(spnew Scene())
{
	m_MayaCamera = m_Scene->GetCamera();
	m_FPSCamera = spnew FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
}

SponzaTest::~SponzaTest()
{

}

void SponzaTest::OnInit(Renderer3D& renderer, Scene& scene)
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
	//ENVIRONMENT MAP
	TextureCube* environment = TextureCube::CreateFromVCross(environmentFiles, 11);

	//SKYPLANE
	Shader* skybox = Shader::CreateFromFile("Skybox", String("/shaders/Skybox") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	Material* skyboxMaterial = spnew Material(skybox);
	skyboxMaterial->SetRenderFlag(Material::RenderFlags::DISABLE_DEPTH_TEST);
	skybox->Bind();
	m_SkyboxMaterial = spnew MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = spnew Entity(MeshFactory::CreateQuad(-1, -1, 2, 2, m_SkyboxMaterial));
	m_Scene->Add(skyboxEntity);

	//PBR SHADER
	Shader* pbrShader = Shader::CreateFromFile("AdvancedLighting", String("/shaders/AdvancedLighting") + (API::Context::GetRenderAPI() == RenderAPI::OPENGL ? ".shader" : ".hlsl"));
	ShaderManager::Add(pbrShader);


	//Models parts
	std::vector<std::string> models = { "arch", "backplate", "walls1", "walls2", "walls3", "ceiling", "column1", "column2", "column3", "curtain_blue", "curtain_green", "curtain_red", "details",
										"fabric_blue", "fabric_green", "fabric_red", "floor", "floor2", "chain", "lion", "pole", "roof", "vase", "vase_round", "vase_hanging","flower", "flower2" };
	//Set texture Wrap to work on OpenGL (or there are texture glitches when texture is repeating)
	Texture::SetWrap(TextureWrap::REPEAT);
	//Loop
	for (int i = 0; i < models.size(); i++) {
		PBRMaterial* material = spnew PBRMaterial(pbrShader);
		material->SetEnviromentMap(environment);
		{
			TextureLoadOptions options(false, true);
			material->SetAlbedoMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/albedo.tga", options));
			material->SetSpecularMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/specular.tga", options));
			material->SetGlossMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/gloss.tga", options));
			material->SetNormalMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/normal.tga", options));
		}

		Model* model = spnew Model("res/Sponza/" + models[i] + "/model.spm", spnew MaterialInstance(material));
		Entity* entity = spnew Entity(model->GetMesh(), mat4::Translate(vec3(0, 0, 0))* mat4::Rotate(-90, vec3::XAxis()));
		m_Scene->Add(entity);
	}
	//Reset it
	Texture::SetWrap(TextureWrap::CLAMP);

	//LIGHT(S)
	LightSetup* lights = spnew LightSetup();
	lights->Add(spnew Light(vec3(0.8f)));
	m_Scene->PushLightSetup(lights);

	DebugMenu::Add("SponzaTest/Light Direction", &lights->GetLights()[0]->direction, -1.0f, 1.0f);
	DebugMenu::Add("SponzaTest/Light Intensity", &lights->GetLights()[0]->intensity, 0, 100);
}

void SponzaTest::OnTick()
{
}

void SponzaTest::OnUpdate(const Timestep& ts)
{
	// Still OpenGL maths style (column-major)
	mat4 vp = m_Scene->GetCamera()->GetProjectionMatrix() * m_Scene->GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));
}

void SponzaTest::OnEvent(sp::events::Event& event)
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

void SponzaTest::OnRender(Renderer3D& renderer)
{
	Layer3D::OnRender(renderer);
}
