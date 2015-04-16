#include "sound_manager.h"

namespace sparky { namespace audio {

	extern "C" void SoundManager_add(int id, const char* name, const char* filename);

#ifdef SPARKY_EMSCRIPTEN
	std::map<std::string, int> SoundManager::m_SoundIDs;
	int SoundManager::s_SID = 0;
#else
	gau_Manager* SoundManager::m_Manager = nullptr;
	ga_Mixer* SoundManager::m_Mixer = nullptr;
#endif

	std::vector<Sound*> SoundManager::m_Sounds;

	void SoundManager::init()
	{
#ifdef SPARKY_EMSCRIPTEN
		EM_ASM(
		window.SoundManager = {};
		window.SoundManager.m_Sounds = { };
		window.SoundManager.add = function(name, filename) { alert('Loading audio file ' + name + ' path=' + filename + '!'); window.SoundManager.m_Sounds[name] = new Audio(filename); };
		window.SoundManager.play = function(name) { window.SoundManager.m_Sounds[name].play(); };
		window.SoundManager.pause = function(name) { window.SoundManager.m_Sounds[name].pause(); };
		window.SoundManager.stop = function(name) { window.SoundManager.pause(name); window.SoundManager.m_Sounds[name].currentTime = 0; window.SoundManager.m_Sounds[name].loop = false; };
		window.SoundManager.loop = function(name) { window.SoundManager.m_Sounds[name].play(); window.SoundManager.m_Sounds[name].loop = true; };
		window.SoundManager.setGain = function(name, gain) { window.SoundManager.m_Sounds[name].volume = gain; };

		var func_map = {
			0: window.SoundManager.add
		}

		function _SoundManager_add(id, name, filename) {
			func_map[id](Pointer_stringify(name), Pointer_stringify(filename));
 		}
		);
#else
		gc_initialize(0);
		m_Manager = gau_manager_create();
		m_Mixer = gau_manager_mixer(m_Manager);
#endif
	}

	void SoundManager::add(Sound* sound)
	{
		m_Sounds.push_back(sound);
		m_SoundIDs[sound->getName()] = s_SID++;
		SoundManager_add(0, sound->getName().c_str(), sound->getFileName().c_str());
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