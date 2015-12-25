#include "SoundManager.h"

namespace SparkyCLI {

	void SoundManager::Init()
	{
		sparky::audio::SoundManager::Init();
	}

	Sound^ SoundManager::Add(Sound^ sound)
	{
		sparky::audio::SoundManager::Add(sound->GetHandle());
		return sound;
	}

	Sound^ SoundManager::Get(System::String^ name)
	{
		return gcnew Sound(sparky::audio::SoundManager::Get(string_to_std_string(name)));
	}

	void SoundManager::Update()
	{
		sparky::audio::SoundManager::Update();
	}

	void SoundManager::Clean()
	{
		sparky::audio::SoundManager::Clean();
	}

}