#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "sp/app/Window.h"
#include "sp/graphics/layers/Layer.h"
#include "sp/utils/Timer.h"
#include "sp/utils/Timestep.h"

#include "sp/events/Events.h"

#include "sp/graphics/API/Context.h"

namespace sp {

	namespace debug {
		class DebugLayer;
	}

	class SP_API Application
	{
	private:
		static Application* s_Instance;
	public:
		Window* window;
		debug::DebugLayer* m_DebugLayer;
	private:
		bool m_Running, m_Suspended;
		Timer* m_Timer;
		uint m_UpdatesPerSecond, m_FramesPerSecond;
		float m_Frametime;

		String m_Name;
		WindowProperties m_Properties;

		std::vector<graphics::Layer*> m_LayerStack;
		std::vector<graphics::Layer*> m_OverlayStack;
	public:
		Application(const String& name, const WindowProperties& properties, graphics::API::RenderAPI api = graphics::API::RenderAPI::OPENGL);
		virtual ~Application();

		virtual void Init();

		void PushLayer(graphics::Layer* layer);
		graphics::Layer* PopLayer();
		graphics::Layer* PopLayer(graphics::Layer* layer);

		void PushOverlay(graphics::Layer* layer);
		graphics::Layer* PopOverlay();
		graphics::Layer* PopOverlay(graphics::Layer* layer);

		void Start();
		void Suspend();
		void Resume();
		void Stop();

		inline uint GetFPS() const { return m_FramesPerSecond; }
		inline uint GetUPS() const { return m_UpdatesPerSecond; }
		inline float GetFrametime() const { return m_Frametime; }

		inline uint GetWindowWidth() const { return window->GetWidth(); }
		inline uint GetWindowHeight() const { return window->GetHeight(); }
		inline maths::vec2 GetWindowSize() const { return maths::vec2((float)window->GetWidth(), (float)window->GetHeight()); }

		String GetBuildConfiguration();
		String GetPlatform(); // TODO: Return "Platform" object rather than String
	private:
		void PlatformInit();
		void Run();

		void OnTick();
		void OnUpdate(const Timestep& ts);
		void OnRender();
	private:
		void OnEvent(events::Event& event);
	public:
		inline static Application& GetApplication() { return *s_Instance; }
	};


}