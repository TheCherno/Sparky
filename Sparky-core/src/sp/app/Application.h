#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "sp/app/Window.h"
#include "sp/graphics/layers/Layer.h"
#include "sp/utils/Timer.h"

#include "sp/events/Events.h"

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

		const char* m_Name;
		uint m_Width, m_Height;

		std::vector<graphics::Layer*> m_LayerStack;
		std::vector<graphics::Layer*> m_OverlayStack;
	public:
		Application(const char* name, uint width, uint height);
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

		const uint GetFPS() const { return m_FramesPerSecond; }
		const uint GetUPS() const { return m_UpdatesPerSecond; }
	private:
		void PlatformInit();
		void Run();

		void OnTick();
		void OnUpdate();
		void OnRender();
	private:
		void OnEvent(events::Event& event);
	public:
		inline static Application& GetApplication() { return *s_Instance; }
	};


}