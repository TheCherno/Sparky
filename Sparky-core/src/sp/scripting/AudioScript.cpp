#include <lua.hpp>

#include "sp/audio/SoundManager.h"
#include "sp/audio/Sound.h"

#include "sp/sp.h"
#include "AudioScript.h"

using namespace sp;
using namespace audio;

int CreateSoundClass(lua_State *L) {

}

int AddSound(lua_State *L) {
	int nargs = lua_gettop(L);
	const char* name = lua_tostring(L, 1);
	const char* source = lua_tostring(L, 2);
	SoundManager::Add(new Sound(name, source));
	return 0;
}