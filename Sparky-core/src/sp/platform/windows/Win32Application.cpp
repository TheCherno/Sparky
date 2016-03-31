#include "sp/sp.h"
#include "sp/app/Application.h"

namespace sp {

	Application::Application(const char* name, uint width, uint height, graphics::API::RenderAPI api)
		: m_Name(name), m_InitialWidth(width), m_InitialHeight(height), m_Frametime(0.0f)
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
		window = new Window(m_Name, m_InitialWidth, m_InitialHeight);
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
		while (m_Running)
		{
			window->Clear();
			if (m_Timer->Elapsed() - updateTimer > updateTick)
			{
				OnUpdate();
				updates++;
				updateTimer += updateTick;
				{
					Timer frametime;
					OnRender();
					frames++;
					m_Frametime = frametime.ElapsedMillis();
				}
				window->Update();
			}
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

}