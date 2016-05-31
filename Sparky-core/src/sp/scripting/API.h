#pragma once

#ifndef LUA_LOADED
#define LUA_LOADED
#include <lua.hpp>
#include <luabridge\LuaBridge.h>
#endif

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

namespace sp { namespace scripting { 
	
	void Load(lua_State* L);

} } 