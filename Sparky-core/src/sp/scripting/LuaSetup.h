#pragma once

#include "sp/sp.h"

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

namespace sp { namespace scripting {

	lua_State* Lua_State;

	class SP_API LuaSetup
	{
	public:
		LuaSetup() {}
		~LuaSetup() {}

		static lua_State* CreateState(lua_State** state);
		static void Init(lua_State* state);

		static void LoadSparkyAPI(lua_State* state);

		static void LoadFile(lua_State* state, const char* filename);
		
		static void Register(lua_State* state, const char* functionname, lua_CFunction functionpointer);
	};
} }

