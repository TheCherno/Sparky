#include "Window.h"

#include "../embedded/Embedded.h"

namespace sp { namespace graphics {

	std::map<void*, Window*> Window::s_Handles;

	Window::Window(const char *title, uint width, uint height)
		: m_Title(title), m_Width(width), m_Height(height), m_Handle(nullptr), m_Closed(false)
	{
		if (!Init())
		{
			SPARKY_ERROR("Failed base Window initialization!");
			return;
		}

#ifdef SPARKY_PLATFORM_WEB
		FontManager::Add(new Font("SourceSansPro", "res/SourceSansPro-Light.ttf", 32));
#else
		FontManager::Add(new Font("SourceSansPro", internal::DEFAULT_FONT, internal::DEFAULT_FONT_SIZE, 32));
#endif

#ifdef SPARKY_PLATFORM_WEB
		FreeImage_Initialise();
#endif

		audio::SoundManager::Init();

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_Keys[i] = false;
			m_KeyState[i] = false;
			m_KeyTyped[i] = false;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
			m_MouseState[i] = false;
			m_MouseClicked[i] = false;
		}
		m_MouseGrabbed = true;
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
			SPARKY_FATAL("Failed to initialize platform!");
			return false;
		}
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SPARKY_WARN("----------------------------------");
		SPARKY_WARN(" OpenGL:");
		SPARKY_WARN("    ", glGetString(GL_VERSION));
		SPARKY_WARN("    ", glGetString(GL_VENDOR));
		SPARKY_WARN("    ", glGetString(GL_RENDERER));
		SPARKY_WARN("----------------------------------");
		return true;
	}

	bool Window::IsKeyPressed(uint keycode) const
	{
		// TODO: Log this!
		if (keycode >= MAX_KEYS)
			return false;

		return m_Keys[keycode];
	}

	bool Window::IsKeyTyped(uint keycode) const
	{
		// TODO: Log this!
		if (keycode >= MAX_KEYS)
			return false;

		return m_KeyTyped[keycode];
	}
	 
	bool Window::IsMouseButtonPressed(uint button) const
	{
		// TODO: Log this!
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseButtons[button];
	}

	bool Window::IsMouseButtonClicked(uint button) const
	{
		// TODO: Log this!
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseClicked[button];
	}

	const maths::vec2& Window::GetMousePosition() const
	{
		return m_MousePosition;
	}

	const bool Window::IsMouseGrabbed() const
	{
		return m_MouseGrabbed;
	}

	void Window::SetMouseGrabbed(bool grabbed)
	{
		m_MouseGrabbed = grabbed;
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

	void Window::UpdateInput()
	{
		for (int i = 0; i < MAX_KEYS; i++)
			m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];

		for (int i = 0; i < MAX_BUTTONS; i++)
			m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

		memcpy(m_KeyState, m_Keys, MAX_KEYS);
		memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
	}

	bool Window::Closed() const
	{
		return m_Closed;
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

} }