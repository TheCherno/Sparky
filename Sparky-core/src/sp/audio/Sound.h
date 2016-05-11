#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"
#include "sp/maths/vec3.h"
#include "sp/maths/Quaternion.h"

#ifndef SP_PLATFORM_WEB
	struct ga_Sound;
	struct ga_Handle;
#endif

namespace sp { namespace audio {

	class SP_API Sound
	{
	private:
		String m_Name;
		String m_Filename;
		uint m_Count;

		ga_Sound* m_Sound;
		ga_Handle* m_Handle;

		bool m_Playing;
		float m_Gain;
		float m_Pan;
		float m_Pitch;

	public:
		Sound(const String& name, const String& filename);
		~Sound();


		void Play();
		void Loop();
		void Pause();
		void Resume();
		void Stop();

		/*
			Controls volume of sound between 0.0f and 1.0f.
		*/
		void SetGain(float gain);

		/*
			Calculates how high the volume should be based on the
			distance between the observer and the entity emitting
			the sound within a specified radius. The radius
			represents zero volume, while the entity's position
			represents max volume.
		*/
		void SetGainByDistance(const maths::vec3& cameraPosition, const maths::vec3& entityPosition, float maxRadius, float maxVolume, float factor = 100.0f);

		/*
			Slides speaker audio between values of -1.0f (left) and 1.0f (right)
		*/
		void SetPan(float pan);

		/*
			Determine how much to pan the audio in either direction
			based on the angular degree the entity emitting sound is 
			to the face direction of the observer.
		*/
		void SetPanByAngle(const maths::vec3& cameraPosition, const maths::vec3& entityPosition, const maths::vec3& forward, const maths::vec3& up);

		inline const bool IsPlaying() const { return m_Playing; }
		inline const float GetGain() const { return m_Gain; }
		inline const float GetPan() const { return m_Pan; }
		inline const String& GetName() const { return m_Name; }
		inline const String& GetFileName() const { return m_Filename; }

#ifdef SPARKY_PLATFORM_WEB
#else
		friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);
		friend void loop_on_finish(ga_Handle* in_handle, void* in_context);
#endif

	};

} }