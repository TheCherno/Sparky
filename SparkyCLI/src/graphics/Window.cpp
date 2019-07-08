#include "Window.h"

namespace SparkyCLI {

    Window::Window(sp::Window* instance)
        : ManagedClass(instance)
    {
    }

    Window::Window(System::String^ name, System::UInt32 width, System::UInt32 height)
    {
        m_Instance = new sp::Window(string_to_char_array(name), {width, height, false, 0});
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

    System::UInt32 Window::GetWidth()
    {
        return m_Instance->GetWidth();
    }

    System::UInt32 Window::GetHeight()
    {
        return m_Instance->GetHeight();
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