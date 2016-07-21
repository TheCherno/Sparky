#include "sp/sp.h"
#include "sp/app/Application.h"

namespace sp {

	Application::Application(const String& name, const WindowProperties& properties, graphics::API::RenderAPI api)
		: m_Name(name), m_Properties(properties), m_Frametime(0.0f)
	{
		s_Instance = this;
		graphics::API::Context::SetRenderAPI(api);
	}

	Application::~Application()
	{
		delete window;
	}

	void Application::PlatformInit()
	{
		window = new Window(m_Name, m_Properties);
		window->SetEventCallback(METHOD(&Application::OnEvent));
	}

	void Application::Start()
	{
		Init();
		m_Running = true;
		m_Suspended = false;
		Run();
	}

	void Application::Suspend()
	{
		m_Suspended = true;
	}

	void Application::Resume()
	{
		m_Suspended = false;
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		m_Timer = new Timer();
		float timer = 0.0f;
		float updateTimer = 0.0f;
		float updateTick = 1.0f / 60.0f;
		uint frames = 0;
		uint updates = 0;
		Timestep timestep(m_Timer->ElapsedMillis());
		while (m_Running)
		{
			window->Clear();
			float now = m_Timer->ElapsedMillis();
			if (now - updateTimer > updateTick)
			{
				timestep.Update(now);
				OnUpdate(timestep);
				updates++;
				updateTimer += updateTick;
			}
			{
				Timer frametime;
				OnRender();
				frames++;
				m_Frametime = frametime.ElapsedMillis();
			}
			window->Update();
			if (m_Timer->Elapsed() - timer > 1.0f)
			{
				timer += 1.0f;
				m_FramesPerSecond = frames;
				m_UpdatesPerSecond = updates;
				frames = 0;
				updates = 0;
				OnTick();
			}
			if (window->Closed())
				m_Running = false;
		}
	}

	String Application::GetPlatform()
	{
#if defined(SP_PLATFORM_WIN32)
		return "Win32";
#elif defined(SP_PLATFORM_WIN64)
		return "Win64";
#else
		return "Unknown Platform";
#endif
	}

}