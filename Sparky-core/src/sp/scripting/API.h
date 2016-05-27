#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

#include "Sparky.h"

namespace sp { namespace scripting { 
	
	using namespace audio;

	void Load(lua_State* L)
	{
		std::vector<luaL_Reg> soundTable;
		std::vector<luaL_Reg> soundMetaTable;
		soundMetaTable.push_back(LUAM_METHOD(Sound, Play));
		soundMetaTable.push_back(LUAM_METHOD(Sound, Stop));
		soundMetaTable.push_back(LUAM_METHOD(Sound, Loop));
		soundMetaTable.push_back(LUAM_METHOD(Sound, Resume));
		soundMetaTable.push_back(LUAM_METHOD(Sound, Pause));
		soundMetaTable.push_back(LUAM_METHOD(Sound, IsPlaying));
		soundMetaTable.push_back(LUAM_METHOD(Sound, GetFileName));
		soundMetaTable.push_back(LUAM_METHOD(Sound, GetName));
		soundMetaTable.push_back(LUAM_METHOD(Sound, GetGain));
		soundMetaTable.push_back(LUAM_METHOD(Sound, SetGain));

		LUAM_CLASSREGISTER(L, Sound, soundTable, soundMetaTable, LUAM_CONSTRUCTOR(Sound, std::string, std::string));

		std::vector<luaL_Reg> soundManagerTable;
		std::vector<luaL_Reg> soundManagerMetaTable;
		//soundManagerTable.push_back(LUAM_METHOD(SoundManager, Add));
		//soundManagerTable.push_back(LUAM_METHOD(SoundManager, Get));

		LUAM_CLASSREGISTERNOCONS(L, SoundManager, soundManagerTable, soundManagerMetaTable);
	}

} } 