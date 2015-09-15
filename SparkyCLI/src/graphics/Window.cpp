#include "Window.h"

namespace SparkyCLI {

	Window::Window(sparky::graphics::Window* instance)
		: ManagedClass(instance)
	{
	}

	Window::Window(System::String^ name, System::UInt32 width, System::UInt32 height)
	{
		m_Instance = new sparky::graphics::Window(string_to_char_array(name), width, height);
	}

	void Window::Clear()
	{
		m_Instance->Clear();
	}

	void Window::Update()
	{
		m_Instance->Update();
	}

	void Window::UpdateInput()
	{
		m_Instance->UpdateInput();
	}

	bool Window::Closed()
	{
		return m_Instance->Closed();
	}

	System::UInt32 Window::GetWidth()
	{
		return m_Instance->GetWidth();
	}

	System::UInt32 Window::GetHeight()
	{
		return m_Instance->GetHeight();
	}

	bool Window::IsKeyPressed(System::UInt32 keycode)
	{
		return m_Instance->IsKeyPressed(keycode);
	}

	bool Window::IsKeyTyped(System::UInt32 keycode)
	{
		return m_Instance->IsKeyTyped(keycode);
	}

	bool Window::IsMouseButtonPressed(System::UInt32 button)
	{
		return m_Instance->IsMouseButtonPressed(button);
	}

	bool Window::IsMouseButtonClicked(System::UInt32 button)
	{
		return m_Instance->IsMouseButtonClicked(button);
	}

	Vector2^ Window::GetMousePosition()
	{
		sparky::maths::vec2 position = m_Instance->GetMousePosition();
		return gcnew Vector2(position.x, position.y);
	}

	void Window::SetVsync(bool enabled)
	{
		return m_Instance->SetVsync(enabled);
	}

	bool Window::IsVsync()
	{
		return m_Instance->IsVsync();
	}

}