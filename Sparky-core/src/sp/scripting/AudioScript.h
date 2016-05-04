#pragma once

struct lua_State;
struct luaL_Reg;

namespace sp {
	namespace audio {
		class Sound;
		class SoundManager;
	}
}

namespace sp { namespace scripting { namespace audio {

	int L_Register_Audio_Classes(lua_State *L);

} } }
