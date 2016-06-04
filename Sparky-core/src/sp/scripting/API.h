#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

#include "Sparky.h"

namespace sp { namespace scripting { 

	void Load(lua_State* L)
	{
		_LUAM_FUNCTION(audio, Sound, Play);
		_LUAM_FUNCTION(audio, Sound, Stop);
		_LUAM_FUNCTION(audio, Sound, Loop);
		_LUAM_FUNCTION(audio, Sound, Resume);
		_LUAM_FUNCTION(audio, Sound, Pause);
		LUAM_FUNCTION(audio, Sound, IsPlaying, bool);
		LUAM_FUNCTION(audio, Sound, GetFileName, String);
		LUAM_FUNCTION(audio, Sound, GetName, String);
		LUAM_FUNCTION(audio, Sound, GetGain, float);
		_LUAM_FUNCTION(audio, Sound, SetGain, float);

		LUAM_CLASSREGISTER(audio, Sound, std::string, std::string);

		LUAM_STATICFUNCTION(audio, SoundManager, Get, audio::Sound, String);
		_LUAM_STATICFUNCTION(audio, SoundManager, Add, audio::Sound);

		_LUAM_CLASSREGISTER(audio, SoundManager);
	}

} } 