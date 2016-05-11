#include "sp/sp.h"
#include "Sound.h"
#include "SoundManager.h"
#include "sp/maths/maths_func.h"

#include <iomanip>

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
		std::vector<String> split = SplitString(m_Filename, '.');
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

	void Sound::SetGainByDistance(const maths::vec3 & cameraPosition, const maths::vec3 & entityPosition, float maxRadius, float maxVolume, float factor)
	{
		// calculate the distance between the positions of the camera in relations to the source creating sound.
		maths::vec3& difference = cameraPosition - entityPosition;
		const float DISTANCE_FROM_ENTITY = maths::sqrt(difference.Dot(difference));
		
		// We need to find out based on the radius from the source what percentage
		// in distance that the camera is from the max value of the radius.
		const float PERCENTAGE = ((DISTANCE_FROM_ENTITY * 10.0f) / maxRadius);

		// Convert the percentage into a 0.0f to 1.0f value to set the gain.
		const float volume = maxVolume - (PERCENTAGE / (maxVolume * factor));
		SetGain(maths::clamp(volume, 0.0f, 1.0f));
	}

	void Sound::SetPan(float pan)
	{
		if (!m_Playing)
		{
			std::cout << "[Sound] Cannot set pan! Sound is not currently playing!" << std::endl;
			return;
		}
		m_Pan = pan;
#ifdef SPARKY_PLATFORM_WEB
		SoundManagerSetPan(m_Name.c_str(), pan);
#else
		ga_handle_setParamf(m_Handle, GA_HANDLE_PARAM_PAN, pan);
#endif
	}

	void Sound::SetPanByAngle(const maths::vec3& cameraPosition, const maths::vec3& entityPosition, const maths::vec3& forward, const maths::vec3& up)
	{
		/* 
			NOTE: 270 to 90 degrees (clockwise) are between a value of -50 and 50 with 0 
			being the direction the camera pointed. The lower half (90 to 270) is vertically
			flipped to maintain smooth transition while moving.
			
			TODO: Objects behind the camera position should be reduced by a scaling percentage
			to mimic real-world hearing perception.
		*/
		maths::vec3& direction = forward.Cross(up);
		direction = forward.Normalize();
		maths::vec3& distanceBetween = entityPosition - cameraPosition;
		distanceBetween = distanceBetween.Normalize();
		const float ANGLE = direction.Dot(distanceBetween);

		/* 
			Determine how much to pan audio left, center and right
			-1-----0-----1
			 L-----C-----R
		*/
		const float PAN_UNIT = (2.0f / 180.0f);	
		const float ANGLE_OFFSET = 180.0f / 100.0f * maths::toDegrees(ANGLE);
		const float PAN_VALUE = maths::clamp(ANGLE_OFFSET * PAN_UNIT, -1.0f, 1.0f);

		SetPan(PAN_VALUE);
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