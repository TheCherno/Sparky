#include "Window.h"

namespace sp { namespace cli {

	Window::Window(sp::Window* instance)
		: ManagedClass(instance)
	{
	}

	Window::Window(System::String^ name, WindowProperties properties)
	{
		m_Instance = new sp::Window(StringToStdString(name), *(sp::WindowProperties*)&properties);
	}

	void Window::Clear()
	{
		m_Instance->Clear();
	}

	void Window::Update()
	{
		m_Instance->Update();
	}

	bool Window::Closed()
	{
		return m_Instance->Closed();
	}

	void Window::SetTitle(System::String^ title)
	{
		m_Instance->SetTitle(StringToStdString(title));
	}

	uint Window::GetWidth()
	{
		return m_Instance->GetWidth();
	}

	uint Window::GetHeight()
	{
		return m_Instance->GetHeight();
	}

	void Window::SetVsync(bool enabled)
	{
		m_Instance->SetVsync(enabled);
	}

	bool Window::IsVsync()
	{
		return m_Instance->IsVsync();
	}

	InputManager^ Window::GetInputManager()
	{
		return m_Instance->GetInputManager();
	}

	Window^ Window::GetWindowClass()
	{
		return gcnew Window(sp::Window::GetWindowClass(nullptr));
	}

} }