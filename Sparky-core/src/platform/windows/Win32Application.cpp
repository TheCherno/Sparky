#include <app/Application.h>

namespace sparky {

	Application::Application(const char* name, uint width, uint height)
		: m_Name(name), m_Width(width), m_Height(height)
	{

	}

	Application::~Application()
	{
		delete window;
	}

	void Application::Init()
	{
		window = new graphics::Window(m_Name, m_Width, m_Height);
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

	void Application::Tick()
	{

	}

	void Application::Update()
	{

	}

	void Application::Render()
	{

	}

	void Application::Run()
	{
		m_Timer = new Timer();
		float timer = 0.0f;
		float updateTimer = 0.0f;
		float updateTick = 1.0f / 60.0f;
		unsigned int frames = 0;
		unsigned int updates = 0;
		while (m_Running)
		{
			window->Clear();
			if (m_Timer->Elapsed() - updateTimer > updateTick)
			{
				window->UpdateInput();
				Update();
				updates++;
				updateTimer += updateTick;
			}
			Render();
			frames++;
			window->Update();
			if (m_Timer->Elapsed() - timer > 1.0f)
			{
				timer += 1.0f;
				m_FramesPerSecond = frames;
				m_UpdatesPerSecond = updates;
				frames = 0;
				updates = 0;
				Tick();
			}
			if (window->Closed())
				m_Running = false;
		}
	}

}