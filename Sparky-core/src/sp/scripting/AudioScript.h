#pragma once

struct lua_State;
struct luaL_Reg;

namespace sp {
	namespace audio {
		class Sound;
	}
}

namespace sp { namespace scripting { namespace audio {


	sp::audio::Sound* L_Sound_New(lua_State *L);

	int L_Sound_Play(lua_State *L);
	int L_Sound_Loop(lua_State *L);
	int L_Sound_Pause(lua_State *L);
	int L_Sound_Stop(lua_State *L);
	int L_Sound_Resume(lua_State *L);
	int L_Sound_GetGain(lua_State *L);
	int L_Sound_SetGain(lua_State *L);

	int L_Register_Class(lua_State *L);

} } }
