#include "sp/sp.h"
#include "LuaSetup.h"

#include <lua.hpp>
#include <luabind\luabind.hpp>

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
			luaL_dofile(state, physicalPath.c_str());
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

	void LuaSetup::ErrorPrint(lua_State* state)
	{
		luabind::object msg( luabind::from_stack(state, -1 ) );
		std::ostringstream str;
		str << "lua> run-time error: " << msg;
		SP_ERROR( str.str() );

		std::string traceback = luabind::call_function<std::string>( luabind::globals(state)["debug"]["traceback"] );
		traceback = std::string( "lua> " ) + traceback;
		SP_ERROR( traceback.c_str() );
	}

	void LuaSetup::LoadSparkyAPI(lua_State* state)
	{
		Load(state);
	}
} }