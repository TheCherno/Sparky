#include "sp/sp.h"
#include "Window.h"

#include "sp/graphics/Renderer.h"

#include "sp/utils/Log.h"

#include "sp/embedded/Embedded.h"
#include <FreeImage.h>

namespace sp {

	using namespace graphics;

	std::map<void*, Window*> Window::s_Handles;

	Window::Window(const String& title, const WindowProperties& properties)
		: m_Title(title), m_Properties(properties), m_Handle(nullptr), m_Closed(false), m_EventCallback(nullptr)
	{
		if (!Init())
		{
			SP_ERROR("Failed base Window initialization!");
			return;
		}

#ifdef SPARKY_PLATFORM_WEB
		FontManager::Add(new Font("SourceSansPro", "res/SourceSansPro-Light.ttf", 32));
#else
		FontManager::SetScale(maths::vec2(m_Properties.width / 32.0f, m_Properties.height / 18.0f)); // TODO: Seriously
		FontManager::Add(new Font("SourceSansPro", internal::DEFAULT_FONT, internal::DEFAULT_FONT_SIZE, 32));
#endif

		FreeImage_Initialise();

		audio::SoundManager::Init();
		m_InputManager = new InputManager();
	}

	Window::~Window()
	{
		FontManager::Clean();
		TextureManager::Clean();
		audio::SoundManager::Clean();
	}

	bool Window::Init()
	{
		if (!PlatformInit())
		{
			SP_FATAL("Failed to initialize platform!");
			return false;
		}

		Renderer::Init();
		
		SetTitle(m_Title);
		return true;
	}
	
	void Window::SetVsync(bool enabled)
	{
		// TODO: Not implemented
		m_Vsync = enabled;
		
	}

	void Window::Clear() const
	{
		Renderer::Clear(RENDERER_BUFFER_COLOR | RENDERER_BUFFER_DEPTH);
	}

 	void Window::Update()
	{
		PlatformUpdate();
		audio::SoundManager::Update();
	}

	bool Window::Closed() const
	{
		return m_Closed;
	}

	void Window::SetEventCallback(const WindowEventCallback& callback)
	{
		m_EventCallback = callback;
		m_InputManager->SetEventCallback(m_EventCallback);
	}

	void Window::RegisterWindowClass(void* handle, Window* window)
	{
		s_Handles[handle] = window;
	}

	Window* Window::GetWindowClass(void* handle)
	{
		if (handle == nullptr)
			return s_Handles.begin()->second;

		return s_Handles[handle];
	}

}