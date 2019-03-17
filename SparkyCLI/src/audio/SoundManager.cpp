#include "SoundManager.h"

namespace SparkyCLI {

	void SoundManager::Init()
	{
		sp::audio::SoundManager::Init();
	}

	Sound^ SoundManager::Add(Sound^ sound)
	{
		sp::audio::SoundManager::Add(sound->GetHandle());
		return sound;
	}

	Sound^ SoundManager::Get(System::String^ name)
	{
		return gcnew Sound(sp::audio::SoundManager::Get(string_to_std_string(name)));
	}

	void SoundManager::Update()
	{
		sp::audio::SoundManager::Update();
	}

	void SoundManager::Clean()
	{
		sp::audio::SoundManager::Clean();
	}

}