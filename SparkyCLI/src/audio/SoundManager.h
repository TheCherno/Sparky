#pragma once

#include "..\SparkyCLI.h"
#include "Sound.h"

#include <audio/SoundManager.h>

namespace SparkyCLI {

	// Static class, no ManagedClass type
	public ref class SoundManager
	{
	public:
		static void Init();
		static Sound^ Add(Sound^ sound);
		static Sound^ Get(System::String^ name);
		static void Update();
		static void Clean();
	};

}