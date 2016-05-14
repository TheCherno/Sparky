#include "sp/sp.h"
#include "LuaSetup.h"

#include <lua.hpp>
#include "luabinddb/luabind.hpp"

#include "sp/utils/Log.h"
#include "sp/system/VFS.h"

#include "API.h"

namespace sp { namespace scripting {

	lua_State* LuaSetup::CreateState(lua_State** state)
	{
		lua_State* s = luaL_newstate();
		*state = s;
		Lua_State = s;
		return s;
	}

	void LuaSetup::Init(lua_State* state)
	{
		luaopen_io(state);
		luaopen_base(state);
		luaopen_table(state);
		luaopen_string(state);
		luaopen_math(state);

		luaL_openlibs(state);
		luabind::open(state);
	}

	void LuaSetup::LoadFile(lua_State* state, const char* filename)
	{
		String physicalPath;
		if (VFS::Get()->ResolvePhysicalPath(filename, physicalPath))
		{
			luaL_loadfile(state, physicalPath.c_str());
			int errors = lua_pcall(state, 0, LUA_MULTRET, 0);
		}
		else {
			SP_ERROR("LUA: Could not find file!", physicalPath.c_str());
		}
	}

	void LuaSetup::Register(lua_State* state, const char* functionname, lua_CFunction functionpointer)
	{
		lua_register(state, functionname, functionpointer);
	}

	void LuaSetup::LoadSparkyAPI(lua_State* state)
	{
		Load(state);
	}
} }