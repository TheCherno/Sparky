#include "sound_manager.h"

namespace sparky { namespace audio {

	extern "C" void SoundManager_add(int id, const char* name, const char* filename);

#ifdef SPARKY_EMSCRIPTEN
#else
	gau_Manager* SoundManager::m_Manager = nullptr;
	ga_Mixer* SoundManager::m_Mixer = nullptr;
#endif

	std::vector<Sound*> SoundManager::m_Sounds;

	void SoundManager::init()
	{
#ifdef SPARKY_EMSCRIPTEN
		EM_ASM(
			function classSoundManager() {
			this.m_Sounds = {};
			this.add = function(name, filename) { this.m_Sounds[name] = new Audio(filename); };
			this.play = function(name) { this.m_Sounds[name].play(); };
			this.pause = function(name) { this.m_Sounds[name].pause(); };
			this.stop = function(name) { this.pause(name); this.m_Sounds[name].currentTime = 0; this.m_Sounds[name].loop = false; };
			this.loop = function(name) { this.m_Sounds[name].play(); this.m_Sounds[name].loop = true; };
			this.setGain = function(name, gain) { this.m_Sounds[name].volume = gain; };
		};

		window.SoundManager = new classSoundManager();
		);
#else
		gc_initialize(0);
		m_Manager = gau_manager_create();
		m_Mixer = gau_manager_mixer(m_Manager);
#endif
	}

	void SoundManager::add(Sound* sound)
	{
#ifdef SPARKY_EMSCRIPTEN
		EM_ASM_ARGS(
		{ window.SoundManager.add(Pointer_stringify($0), Pointer_stringify($1)); },
		sound->getName().c_str(), sound->getFileName().c_str()
		);
#endif
		m_Sounds.push_back(sound);
	}

	Sound* SoundManager::get(const std::string& name)
	{
		for (Sound* sound : m_Sounds)
		{
			if (sound->getName() == name)
			{
				return sound;
			}
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