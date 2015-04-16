#include "sound_manager.h"

namespace sparky { namespace audio {

#ifndef SPARKY_EMSCRIPTEN
	gau_Manager* SoundManager::m_Manager = nullptr;
	ga_Mixer* SoundManager::m_Mixer = nullptr;
#endif

	std::vector<Sound*> SoundManager::m_Sounds;

	void SoundManager::init()
	{
#ifdef SPARKY_EMSCRIPTEN
#else
		gc_initialize(0);
		m_Manager = gau_manager_create();
		m_Mixer = gau_manager_mixer(m_Manager);
#endif
	}

	void SoundManager::add(Sound* sound)
	{
		m_Sounds.push_back(sound);
	}

	Sound* SoundManager::get(const std::string& name)
	{
		for (Sound* sound : m_Sounds)
		{
			if (sound->getName() == name)
				return sound;
		}
		return nullptr;
	}

	void SoundManager::clean()
	{
		for (int i = 0; i < m_Sounds.size(); i++)
			delete m_Sounds[i];

#ifdef SPARKY_EMSCRIPTEN
#else
		gau_manager_destroy(m_Manager);
		gc_shutdown();
#endif
	}

	void SoundManager::update()
	{
#ifdef SPARKY_EMSCRIPTEN
#else
		gau_manager_update(m_Manager);
#endif
	}

} }