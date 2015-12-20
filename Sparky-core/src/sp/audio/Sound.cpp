#include "sp/sp.h"
#include "Sound.h"
#include "SoundManager.h"

#include "sp/utils/StringUtils.h"

#ifdef SPARKY_PLATFORM_WEB
	#include <emscripten/emscripten.h>
#else
	#include <ga.h>
	#include <gau.h>
#endif

namespace sp { namespace audio {

	Sound::Sound(const String& name, const String& filename)
		: m_Name(name), m_Filename(filename), m_Playing(false), m_Count(0)
	{
		std::vector<String> split = utils::SplitString(m_Filename, '.');
		if (split.size() < 2)
		{
			std::cout << "[Sound] Invalid file name '" << m_Filename << "'!" << std::endl;
			return;
		}
#ifdef SPARKY_PLATFORM_WEB
#else
		m_Sound = gau_load_sound_file(filename.c_str(), split.back().c_str());
		if (m_Sound == nullptr)
			std::cout << "[Sound] Could not load file '" << m_Filename << "'!" << std::endl;
#endif
	}

	Sound::~Sound()
	{
#ifdef SPARKY_PLATFORM_WEB
#else
		ga_sound_release(m_Sound);
#endif
	}

	void Sound::Play()
	{
#ifdef SPARKY_PLATFORM_WEB
		SoundManagerPlay(m_Name.c_str());
#else
		gc_int32 quit = 0;
		m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &destroy_on_finish, &quit, NULL);
		m_Handle->sound = this;
		ga_handle_play(m_Handle);
		m_Count++;
#endif
		m_Playing = true;
	}

	void Sound::Loop()
	{
#ifdef SPARKY_PLATFORM_WEB
		SoundManagerLoop(m_Name.c_str());
#else
		gc_int32 quit = 0;
		m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &loop_on_finish, &quit, NULL);
		m_Handle->sound = this;
		ga_handle_play(m_Handle);
#endif
		m_Playing = true;
	}

	void Sound::Resume()
	{
		if (m_Playing)
			return;

		m_Playing = true;
#ifdef SPARKY_PLATFORM_WEB
		SoundManagerPlay(m_Name.c_str());
#else
		ga_handle_play(m_Handle);
#endif
	}

	void Sound::Pause()
	{
		if (!m_Playing)
			return;

		m_Playing = false;
#ifdef SPARKY_PLATFORM_WEB
		SoundManagerPause(m_Name.c_str());
#else
		ga_handle_stop(m_Handle);
#endif
	}

	void Sound::Stop()
	{
		if (!m_Playing)
			return;

#ifdef SPARKY_PLATFORM_WEB
		SoundManagerStop(m_Name.c_str());
#else
		ga_handle_stop(m_Handle);
#endif
		m_Playing = false;
	}

	void Sound::SetGain(float gain)
	{
		if (!m_Playing)
		{
			std::cout << "[Sound] Cannot set gain! Sound is not currently playing!" << std::endl;
			return;
		}
		m_Gain = gain;
#ifdef SPARKY_PLATFORM_WEB
		SoundManagerSetGain(m_Name.c_str(), gain);
#else
		ga_handle_setParamf(m_Handle, GA_HANDLE_PARAM_GAIN, gain);
#endif
	}

#ifdef SPARKY_PLATFORM_WEB
#else
	void destroy_on_finish(ga_Handle* in_handle, void* in_context)
	{
		Sound* sound = (Sound*)in_handle->sound;
		sound->m_Count--;
		if (sound->m_Count == 0)
			sound->Stop();
	}

	void loop_on_finish(ga_Handle* in_handle, void* in_context)
	{
		Sound* sound = (Sound*) in_handle->sound;
		sound->Loop();
		ga_handle_destroy(in_handle);
	}
#endif

} }