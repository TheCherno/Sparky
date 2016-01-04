#include "sp/sp.h"
#include "Window.h"

#include "sp/utils/Log.h"

#include "sp/embedded/Embedded.h"
#include <GL/glew.h>

namespace sp {

	using namespace graphics;

	std::map<void*, Window*> Window::s_Handles;

	Window::Window(const char *title, uint width, uint height)
		: m_Title(title), m_Width(width), m_Height(height), m_Handle(nullptr), m_Closed(false), m_EventCallback(nullptr)
	{
		if (!Init())
		{
			SP_ERROR("Failed base Window initialization!");
			return;
		}

#ifdef SPARKY_PLATFORM_WEB
		FontManager::Add(new Font("SourceSansPro", "res/SourceSansPro-Light.ttf", 32));
#else
		FontManager::SetScale(maths::vec2(m_Width / 32.0f, m_Height / 18.0f));
		FontManager::Add(new Font("SourceSansPro", internal::DEFAULT_FONT, internal::DEFAULT_FONT_SIZE, 32));
#endif

#ifdef SPARKY_PLATFORM_WEB
		FreeImage_Initialise();
#endif

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
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SP_WARN("----------------------------------");
		SP_WARN(" OpenGL:");
		SP_WARN("    ", glGetString(GL_VERSION));
		SP_WARN("    ", glGetString(GL_VENDOR));
		SP_WARN("    ", glGetString(GL_RENDERER));
		SP_WARN("----------------------------------");
		return true;
	}
	
	void Window::SetVsync(bool enabled)
	{
		// TODO: Not implemented
		m_Vsync = enabled;
	}

	void Window::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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