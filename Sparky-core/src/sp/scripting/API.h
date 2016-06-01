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
		LUAM_FUNCTION(sp::audio, Sound, Play);
		LUAM_FUNCTION(sp::audio, Sound, Stop);
		LUAM_FUNCTION(sp::audio, Sound, Loop);
		LUAM_FUNCTION(sp::audio, Sound, Resume);
		LUAM_FUNCTION(sp::audio, Sound, Pause);
		LUAM_FUNCTION(sp::audio, Sound, IsPlaying);
		LUAM_FUNCTION(sp::audio, Sound, GetFileName);
		LUAM_FUNCTION(sp::audio, Sound, GetName);
		LUAM_FUNCTION(sp::audio, Sound, GetGain);
		LUAM_FUNCTION(sp::audio, Sound, SetGain);

		LUAM_CLASSREGISTER(sp::audio, Sound, std::string, std::string);

		_LUAM_CLASSREGISTER(sp::audio, SoundManager);
	}

} } 