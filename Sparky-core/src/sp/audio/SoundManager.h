#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "Sound.h"

#ifdef SPARKY_PLATFORM_WEB
extern "C" void SoundManagerAdd(const char* name, const char* filename);
extern "C" void SoundManagerPlay(const char* name);
extern "C" void SoundManagerPause(const char* name);
extern "C" void SoundManagerStop(const char* name);
extern "C" void SoundManagerLoop(const char* name);
extern "C" void SoundManagerSetGain(const char* name, double gain);
#endif

#ifndef SP_PLATFORM_WEB
	struct gau_Manager;
	struct ga_Mixer;
#endif

namespace sp { namespace audio {

	class SP_API SoundManager
	{
	private:
		friend class Sound;

#ifdef SPARKY_PLATFORM_WEB
#else
		static gau_Manager* m_Manager;
		static ga_Mixer* m_Mixer;
#endif

		static std::vector<Sound*> m_Sounds;
	public:
		static void Init();
		static Sound* Add(Sound* sound);
		static Sound* Get(const String& name);
		static void Update();
		static void Clean();
	private:
		SoundManager() { }
	};

} }