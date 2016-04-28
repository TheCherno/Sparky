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

	Scripting::CreateState(&m_ScriptingState);
	Scripting::Init(m_ScriptingState);
	Scripting::LoadSparkyAPI(m_ScriptingState);
	Scripting::LoadFile(m_ScriptingState, "/scripts/test.lua");

	Scripting::Call(m_ScriptingState, "printVersion");

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
				m_Scene->SetCamera(m_Scene->GetCamera() == m_MayaCameras ? m_FPSCameras : m_MayaCameras);
				break;
			case SP_KEY_P:
				Scripting::Call(m_ScriptingState, "playSound");
				break;
			}
		}
	}
	Layer::OnEvent(event);
}
