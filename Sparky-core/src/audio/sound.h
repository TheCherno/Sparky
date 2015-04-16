#pragma once

#include <iostream>
#include <string>

#include "../utils/stringutils.h"

#ifdef SPARKY_EMSCRIPTEN
	#include <emscripten/emscripten.h>
#else
	#include "../../ext/gorilla-audio/ga.h"
	#include "../../ext/gorilla-audio/gau.h"
#endif

namespace sparky { namespace audio {

	class Sound
	{
	private:
		std::string m_Name;
		std::string m_Filename;

#ifdef SPARKY_EMSCRIPTEN

#else
		ga_Sound* m_Sound;
		ga_Handle* m_Handle;
		gc_int32 m_Position;
#endif

		bool m_Playing;
		float m_Gain;
	public:
		Sound(const std::string& name, const std::string& filename);
		~Sound();

		void play();
		void loop();
		void pause();
		void resume();
		void stop();

		void setGain(float gain);

		inline const bool isPlaying() const { return m_Playing; }
		inline const float getGain() const { return m_Gain; }
		inline const std::string& getName() const { return m_Name; }
		inline const std::string& getFileName() const { return m_Filename; }

#ifdef SPARKY_EMSCRIPTEN
#else
		friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);
		friend void loop_on_finish(ga_Handle* in_handle, void* in_context);
#endif

	};

} }