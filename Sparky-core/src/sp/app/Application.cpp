#include "Application.h"

namespace sp {

	using namespace graphics;

	Application* Application::s_Instance = nullptr;

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.push_back(layer);
		layer->Init();
	}

	Layer* Application::PopLayer()
	{
		Layer* layer = m_LayerStack.back();
		m_LayerStack.pop_back();
		return layer;
	}

	void Application::OnTick()
	{
		for (uint i = 0; i < m_LayerStack.size(); i++)
			m_LayerStack[i]->OnTick();
	}

	void Application::OnUpdate()
	{
		for (uint i = 0; i < m_LayerStack.size(); i++)
			m_LayerStack[i]->OnUpdate();
	}

	void Application::OnRender()
	{
		for (uint i = 0; i < m_LayerStack.size(); i++)
			m_LayerStack[i]->OnRender();
	}

}