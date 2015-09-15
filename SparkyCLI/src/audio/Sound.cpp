#include "Sound.h"

namespace SparkyCLI {

	Sound::Sound(sparky::audio::Sound* instance)
		: ManagedClass(instance)
	{
	}

	Sound::Sound(System::String^ name, System::String^ filename)
	{
		m_Instance = new sparky::audio::Sound(string_to_std_string(name), string_to_std_string(filename));
	}

	void Sound::Play()
	{
		m_Instance->Play();
	}

	void Sound::Loop()
	{
		m_Instance->Loop();
	}

	void Sound::Pause()
	{
		m_Instance->Pause();
	}

	void Sound::Resume()
	{
		m_Instance->Resume();
	}

	void Sound::Stop()
	{
		m_Instance->Stop();
	}

	void Sound::SetGain(float gain)
	{
		m_Instance->SetGain(gain);
	}

	const bool Sound::IsPlaying()
	{
		return m_Instance->IsPlaying();
	}

	const float Sound::GetGain()
	{
		return m_Instance->GetGain();
	}

	const System::String^ Sound::GetName()
	{
		return std_string_to_string(m_Instance->GetName());
	}

	const System::String^ Sound::GetFileName()
	{
		return std_string_to_string(m_Instance->GetFileName());
	}

}