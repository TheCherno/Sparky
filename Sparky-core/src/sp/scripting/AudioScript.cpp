#include "sp/sp.h"
#include "AudioScript.h"

#include <luabind\luabind.hpp>

#include "sp/audio/SoundManager.h"
#include "sp/audio/Sound.h"

#include "sp/system/Memory.h"

using namespace sp;
using namespace audio;

using namespace luabind;

namespace sp { namespace scripting { namespace audio {
	
	int L_Register_Audio_Classes(lua_State *L) {
		module(L)
			[
			class_<Sound>("Sound")
			.def(constructor<std::string, std::string>())
			.def("Play", &Sound::Play)
			.def("Stop", &Sound::Stop)
			.def("Loop", &Sound::Loop)
			.def("Resume", &Sound::Resume)
			.def("Pause", &Sound::Pause)
			.def("GetGain", &Sound::GetGain)
			.def("SetGain", &Sound::SetGain)
			.def("IsPlaying", &Sound::IsPlaying)
			.def("GetFileName", &Sound::GetFileName)
			.def("GetName", &Sound::GetName)
			];
		return 1;
	}

} } }
