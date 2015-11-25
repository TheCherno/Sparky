#include <Windows.h>
#include <Windowsx.h>

#include <sp/graphics/Window.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace sp { namespace graphics {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static HINSTANCE hInstance;
	static HWND hWnd;
	static HDC hDc;

	static PIXELFORMATDESCRIPTOR get_pixel_format()
	{
		PIXELFORMATDESCRIPTOR result = {};
		result.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		result.nVersion = 1;
		result.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		result.iPixelType = PFD_TYPE_RGBA;
		result.cColorBits = 32;
		result.cDepthBits = 24;
		result.cStencilBits = 8;
		result.cAuxBuffers = 0;
		result.iLayerType = PFD_MAIN_PLANE;
		return result;
	}

	bool Window::PlatformInit()
	{
		hInstance = (HINSTANCE)&__ImageBase;

		WNDCLASS winClass = {};
		winClass.hInstance = hInstance; // GetModuleHandle(0);
		winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		winClass.lpfnWndProc = (WNDPROC)WndProc;
		winClass.lpszClassName = "Sparky Win32 Window";
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);

		if (!RegisterClassA(&winClass))
		{
			// TODO: Handle error
			SPARKY_ERROR("Could not register Win32 class!");
			return false;
		}

		RECT size = { 0, 0, m_Width, m_Height };
		AdjustWindowRectEx(&size, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		hWnd = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			winClass.lpszClassName, m_Title,
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			GetSystemMetrics(SM_CXSCREEN) / 2 - m_Width / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - m_Height / 2,
			// TODO: This requires some... attention
			size.right + (-size.left), size.bottom + (-size.top), NULL, NULL, hInstance, NULL);
			
		if (!hWnd)
		{
			SPARKY_ERROR("Could not create window!");
			return false;
		}

		RegisterWindowClass(hWnd, this);

		hDc = GetDC(hWnd);
		PIXELFORMATDESCRIPTOR pfd = get_pixel_format();
		int pixelFormat = ChoosePixelFormat(hDc, &pfd);
		if (pixelFormat)
		{
			if (!SetPixelFormat(hDc, pixelFormat, &pfd))
			{
				SPARKY_ERROR("Failed setting pixel format!");
				return false;
			}
		}
		else
		{
			SPARKY_ERROR("Failed choosing pixel format!");
			return false;
		}

		HGLRC hrc = wglCreateContext(hDc);
		if (hrc)
		{
			if (!wglMakeCurrent(hDc, hrc))
			{
				SPARKY_ERROR("Failed setting OpenGL context!");
				return false;
			}
		}
		else
		{
			SPARKY_ERROR("Failed creating OpenGL context!");
			return false;
		}

		if (glewInit() != GLEW_OK)
		{
			SPARKY_FATAL("Could not initialize GLEW!");
			return false;
		}

		ShowWindow(hWnd, SW_SHOW);
		SetFocus(hWnd);
		// resize

		return true;
	}

	void Window::PlatformUpdate()
	{
		MSG message;
		while (PeekMessage(&message, hWnd, NULL, NULL, PM_REMOVE) > 0)
		{
			if (message.message == WM_QUIT)
			{
				m_Closed = true;
				return;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(hWnd, &mouse);
		m_MousePosition = maths::vec2(mouse.x, mouse.y);

		SwapBuffers(hDc);
	}

	void Window::SetMousePosition(const maths::vec2& position)
	{
		POINT pt = { (LONG)position.x, (LONG)position.y };
		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void Window::SetMouseCursor(int cursor)
	{
		if (cursor == SP_NO_CURSOR)
		{
			SetCursor(SP_NO_CURSOR);
			while (ShowCursor(false) >= 0);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(true);
		}
	}

	void key_callback(Window* window, int key, uint message)
	{
		window->m_Keys[key] = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
	}

	void mouse_button_callback(Window* window, int button, int x, int y)
	{
		bool down = false;
		switch (button)
		{
		case WM_LBUTTONDOWN:
			button = SP_MOUSE_LEFT;
			down = true;
			break;
		case WM_LBUTTONUP:
			button = SP_MOUSE_LEFT;
			down = false;
			break;
		case WM_RBUTTONDOWN:
			button = SP_MOUSE_RIGHT;
			down = true;
			break;
		case WM_RBUTTONUP:
			button = SP_MOUSE_RIGHT;
			down = false;
			break;
		case WM_MBUTTONDOWN:
			button = SP_MOUSE_MIDDLE;
			down = true;
			break;
		case WM_MBUTTONUP:
			button = SP_MOUSE_MIDDLE;
			down = false;
			break;
		}

		window->m_MouseButtons[button] = down;
		window->m_MousePosition.x = x;
		window->m_MousePosition.y = y;
	}

	void resize_callback(Window* window, int width, int height)
	{
		window->m_Width = width;
		window->m_Height = height;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = NULL;
		Window* window = Window::GetWindowClass(hWnd);
		if (window == nullptr)
			return DefWindowProc(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam)) // Is minimized
			{
				// active
			}
			else
			{
				// inactive
			}

			return 0;
		}
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			result = DefWindowProc(hWnd, message, wParam, lParam);
		} break;
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			key_callback(window, wParam, message);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			mouse_button_callback(window, message, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_SIZE:
			resize_callback(window, LOWORD(lParam), HIWORD(lParam));
			break;
		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
		return result;
	}

} }