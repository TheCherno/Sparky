#include "sp/sp.h"
#include "Sound.h"
#include "SoundManager.h"
#include "sp/maths/maths_func.h"
#include "sp/maths/vec2.h"
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
		m_Gain = maxVolume - (PERCENTAGE / (maxVolume * factor));
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

	

	void Sound::SetPanByAngle(const maths::vec3& cameraPosition, const maths::vec3& entityPosition, float yaw, float soundDampeningPct)
	{
		/* 
			Define the reference point to always stay in front of the 
			direction the camera is facing.
		*/
		const maths::vec3 REFERENCE_POINT(
			cameraPosition.x + 20.0f * maths::sin(yaw),
			cameraPosition.y, 
			cameraPosition.z + 20.0f * -maths::cos(yaw));

		// Solve for the angle between the entity, reference point and camera position
		const maths::vec2 A = maths::vec2(entityPosition.x, entityPosition.z);
		const maths::vec2 B = maths::vec2(cameraPosition.x, cameraPosition.z);
		const maths::vec2 C = maths::vec2(REFERENCE_POINT.x, REFERENCE_POINT.z);

		const maths::vec2 AB = B - A;
		const maths::vec2 BC = C - B;

		const float ANGLE = maths::acos(
						-AB.Dot(BC) / (AB.Magnitude() * BC.Magnitude()));

		/*
		Determine how much to pan audio left, center and right.
			-1-----0-----1
			 L-----C-----R
		*/
		float PAN_VALUE = 0.0f;
		const float PAN_UNIT = 1.0f / 90.0f;

		/* 
			Determine which direction camera is rotating and adjust pan.
			Degrees goes from 0 to 180 whether camera is rotating CW or CCW.
		*/
		bool isRotatingLeft = ((B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x)) > 0.0f;
		
		if (!isRotatingLeft && maths::toDegrees(ANGLE) < 90.0f)
		{
			PAN_VALUE = PAN_UNIT * maths::toDegrees(ANGLE);
			PAN_VALUE = -PAN_VALUE;
			m_Gain = maths::clamp(m_Gain, 0.0f, 1.0f);
		}
		else if (!isRotatingLeft && maths::toDegrees(ANGLE) > 90.0f && maths::toDegrees(ANGLE) < 180.0f)
		{
			PAN_VALUE = 1.0f - (-(1.0f - (PAN_UNIT * maths::toDegrees(ANGLE))));
			PAN_VALUE = -PAN_VALUE;
			
			/* 
			        Dampen volume by set percentage when camera is facing a way
			        based on angle. 
			*/
			m_Gain = (m_Gain - (m_Gain / 100.0f * (maths::toDegrees(ANGLE) / 100.0f * soundDampeningPct)));
			printf("Vol: %f\n", m_Volume);
			m_Gain = maths::clamp(m_Gain, 0.0f, 1.0f);
		}
		
		if (isRotatingLeft && maths::toDegrees(ANGLE) < 90.0f)
		{
			PAN_VALUE = -(PAN_UNIT * maths::toDegrees(ANGLE));
			PAN_VALUE = -PAN_VALUE;
			m_Gain = maths::clamp(m_Gain, 0.0f, 1.0f);
		}
		else if (isRotatingLeft && maths::toDegrees(ANGLE) > 90.0f && maths::toDegrees(ANGLE) < 180.0f)
		{
			PAN_VALUE = -1.0f - ((1.0f - (PAN_UNIT * maths::toDegrees(ANGLE))));
			PAN_VALUE = -PAN_VALUE;
			m_Gain = (m_Gain - (m_Gain / 100.0f * (maths::toDegrees(ANGLE) / 100.0f * soundDampeningPct)));
			m_Gain = maths::clamp(m_Gain, 0.0f, 1.0f);
		}
		
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
