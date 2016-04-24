#include "sp/sp.h"
#include "AudioScript.h"

#include <lua.hpp>
#include <luawrapper.hpp>

#include "sp/audio/SoundManager.h"
#include "sp/audio/Sound.h"

#include "sp\utils\Log.h"
#include <typeinfo>

using namespace sp;
using namespace audio;

namespace sp { namespace scripting { namespace audio {

	Sound* L_Sound_New(lua_State *L)
	{
		String name = lua_tostring(L, 1);
		String filename = lua_tostring(L, 2);
		return new Sound(name, filename);
	}

	int L_Sound_Play(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		s->Play();
		return 0;
	}

	int L_Sound_Loop(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		s->Loop();
		return 0;
	}

	int L_Sound_Pause(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		s->Pause();
		return 0;
	}

	int L_Sound_Stop(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		s->Stop();
		return 0;
	}

	int L_Sound_Resume(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		s->Resume();
		return 0;
	}

	int L_Sound_GetGain(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		lua_pushnumber(L, s->GetGain());
		return 1;
	}

	int L_Sound_SetGain(lua_State *L)
	{
		Sound* s = luaW_check<Sound>(L, 1);
		float g = lua_tonumber(L, 2);
		s->SetGain(g);
		return 0;
	}

	luaL_Reg L_Sound_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Sound_MetaTable[] = {
		{ "Play", L_Sound_Play },
		{ "Loop", L_Sound_Loop },
		{ "Pause", L_Sound_Pause },
		{ "Stop", L_Sound_Stop },
		{ "Resume", L_Sound_Resume },
		{ "GetGain", L_Sound_GetGain },
		{ "SetGain", L_Sound_SetGain },
		{ NULL, NULL }
	};
	
	int L_Register_Audio_Classes(lua_State *L) {
		luaW_register<Sound>(L, "Sound", L_Sound_Table, L_Sound_MetaTable, L_Sound_New);
		return 1;
	}

} } }
