#pragma once

#include "..\SparkyCLI.h"

#include <sp/app/Application.h>

namespace SparkyCLI {

	public ref class Application : public ManagedClass<sp::Application>
	{
	public:
		Application(System::String^ name, uint width, uint height);
		virtual void Init();
		void Start();
		void Suspend();
		void Resume();
		void Stop();

		virtual void Tick();
		virtual void Update();
		virtual void Render();

		const uint GetFPS();
		const uint GetUPS();
	};

}