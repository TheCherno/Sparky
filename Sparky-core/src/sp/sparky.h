#pragma once

#ifdef SPARKY_PLATFORM_WEB
	#include <emscripten/emscripten.h>
#endif

#include "graphics/Label.h"
#include "graphics/Sprite.h"
#include "graphics/Renderer2D.h"
#include "graphics/Batchrenderer2D.h"
#include "graphics/layers/Layer.h"
#include "graphics/layers/Group.h"
#include "graphics/TextureManager.h"
#include "graphics/Window.h"

#include "graphics/shaders/Shader.h"
#include "graphics/shaders/ShaderFactory.h"

#include "audio/Sound.h"
#include "audio/SoundManager.h"

#include "maths/maths.h"
#include "utils/Log.h"
#include "utils/Timer.h"

#ifdef SPARKY_PLATFORM_WEB

static void dispatch_main(void* fp)
{
	std::function<void()>* func = (std::function<void()>*)fp;
	(*func)();
}

#endif

namespace sp {

	class Sparky
	{
	private:
		graphics::Window* m_Window;
		Timer* m_Timer;
		unsigned int m_FramesPerSecond, m_UpdatesPerSecond;
	protected:
		Sparky()
			: m_FramesPerSecond(0), m_UpdatesPerSecond(0)
		{

		}

		virtual ~Sparky()
		{ 
			delete m_Timer;
			delete m_Window;
		}

		graphics::Window* CreateWindow(const char *name, int width, int height)
		{
			m_Window = new graphics::Window(name, width, height);
			return m_Window;
		}

	public:
		void Start()
		{
			Init();
			Run();
		}

	protected:
		// Runs once upon initialization
		virtual void Init() = 0;
		// Runs once per second
		virtual void Tick() { }
		// Runs 60 times per second
		virtual void Update() { }
		// Runs as fast as possible (unless vsync is enabled)
		virtual void Render() = 0;

		const unsigned int GetFPS() const { return m_FramesPerSecond; }
		const unsigned int GetUPS() const { return m_UpdatesPerSecond; }
	private:
		void Run()
		{
			m_Timer = new Timer();
			float timer = 0.0f;
			float updateTimer = 0.0f;
			float updateTick = 1.0f / 60.0f;
			unsigned int frames = 0;
			unsigned int updates = 0;
#ifdef SPARKY_PLATFORM_WEB
			std::function<void()> mainLoop = [&]() {
#else
			while (!m_Window->Closed())
			{
#endif
				m_Window->Clear();
				if (m_Timer->Elapsed() - updateTimer > updateTick)
				{
					m_Window->UpdateInput();
					Update();
					updates++;
					updateTimer += updateTick;
				}
				Render();
				frames++;
				m_Window->Update();
				if (m_Timer->Elapsed() - timer > 1.0f)
				{
					timer += 1.0f;
					m_FramesPerSecond = frames;
					m_UpdatesPerSecond = updates;
					frames = 0;
					updates = 0;
					Tick();
				}
#ifdef SPARKY_PLATFORM_WEB
			};
			emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
			}
#endif
		}
	};

}