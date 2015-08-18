#include <sparkygl.h>
#include <sparky_types.h>

#include <graphics/Window.h>
#include <utils/Timer.h>

namespace sparky {

	class Application
	{
	public:
		graphics::Window* window;
	private:
		bool m_Running, m_Suspended;
		Timer* m_Timer;
		uint m_UpdatesPerSecond, m_FramesPerSecond;

		const char* m_Name;
		uint m_Width, m_Height;
	public:
		Application(const char* name, uint width, uint height);
		virtual ~Application();

		virtual void Init();
		
		void Start();
		void Suspend();
		void Resume();
		void Stop();

		virtual void Tick();
		virtual void Update();
		virtual void Render();

		const uint GetFPS() const { return m_FramesPerSecond; }
		const uint GetUPS() const { return m_UpdatesPerSecond; }
	private:
		void Run();
	};


}