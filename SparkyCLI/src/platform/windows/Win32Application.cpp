#include "../../app/Application.h"

namespace SparkyCLI {

	Application::Application(System::String^ name, uint width, uint height)
	{
		m_Instance = new sparky::Application(string_to_char_array(name), width, height);
	}

	void Application::Init()
	{
		m_Instance->Init();
	}

	void Application::Start()
	{
		m_Instance->Start();
	}

	void Application::Suspend()
	{
		m_Instance->Suspend();
	}

	void Application::Resume()
	{
		m_Instance->Resume();
	}

	void Application::Stop()
	{
		m_Instance->Stop();
	}

	void Application::Tick()
	{
		m_Instance->Tick();
	}

	void Application::Update()
	{
		m_Instance->Update();
	}

	void Application::Render()
	{
		m_Instance->Render();
	}

	const uint Application::GetFPS()
	{
		return m_Instance->GetFPS();
	}

	const uint Application::GetUPS()
	{
		return m_Instance->GetUPS();
	}

}