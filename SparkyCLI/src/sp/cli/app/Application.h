#pragma once

#include <sp/app/Application.h>

#include "sp/cli/SparkyCLI.h"
#include "sp/cli/app/Window.h"
#include "sp/cli/graphics/Context.h"

namespace sp { namespace cli {

	public ref class Application : public ManagedClass<sp::Application>
	{
	internal:
		Application(sp::Application* instance);
	public:
		Application(System::String^ name, WindowProperties properties);
		Application(System::String^ name, WindowProperties properties, RenderAPI api);

		virtual void Init();

		void PushLayer(sp::cli::graphics::Layer^ layer);
		sp::cli::graphics::Layer^ PopLayer();
		sp::cli::graphics::Layer^ PopLayer(sp::cli::graphics::Layer^ layer);

		void PushOverlay(sp::cli::graphics::Layer^ layer);
		sp::cli::graphics::Layer^ PopOverlay();
		sp::cli::graphics::Layer^ PopOverlay(sp::cli::graphics::Layer^ layer);

		void Start();
		void Suspend();
		void Resume();
		void Stop();

		uint GetFPS();
		uint GetUPS();
		float GetFrametime();

		uint GetWindowWidth();
		uint GetWindowHeight();
		maths::vec2 GetWindowSize();

		System::String^ GetBuildConfiguration();
		System::String^ GetPlatform();
	};

} }