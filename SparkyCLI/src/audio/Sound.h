#pragma once

#include "..\SparkyCLI.h"

#include <audio/Sound.h>

namespace SparkyCLI {

	public ref class Sound : public ManagedClass<sparky::audio::Sound>
	{
	public:
		Sound(sparky::audio::Sound* instance);
		Sound(System::String^ name, System::String^ filename);

		void Play();
		void Loop();
		void Pause();
		void Resume();
		void Stop();

		void SetGain(float gain);

		const bool IsPlaying();
		const float GetGain();
		const System::String^ GetName();
		const System::String^ GetFileName();
	};

}