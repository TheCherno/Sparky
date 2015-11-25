#pragma once

#include <iostream>
#include <string>

#include <sp/Types.h>

#include <sp/utils/stringutils.h>

#ifdef SPARKY_PLATFORM_WEB
	#include <emscripten/emscripten.h>
#else
	#include "../../../ext/gorilla-audio/ga.h"
	#include "../../../ext/gorilla-audio/gau.h"
#endif

namespace sp { namespace audio {

	class Sound
	{
	private:
		String m_Name;
		String m_Filename;
		uint m_Count;
#ifdef SPARKY_PLATFORM_WEB
#else
		ga_Sound* m_Sound;
		ga_Handle* m_Handle;
		gc_int32 m_Position;
#endif

		bool m_Playing;
		float m_Gain;
	public:
		Sound(const String& name, const String& filename);
		~Sound();


		void Play();
		void Loop();
		void Pause();
		void Resume();
		void Stop();

		void SetGain(float gain);

		inline const bool IsPlaying() const { return m_Playing; }
		inline const float GetGain() const { return m_Gain; }
		inline const String& GetName() const { return m_Name; }
		inline const String& GetFileName() const { return m_Filename; }

#ifdef SPARKY_PLATFORM_WEB
#else
		friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);
		friend void loop_on_finish(ga_Handle* in_handle, void* in_context);
#endif

	};

} }