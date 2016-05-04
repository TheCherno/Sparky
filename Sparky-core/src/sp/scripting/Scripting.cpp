#include "sp/sp.h"
#include "Scripting.h"

#include <lua.hpp>
#include <luabind\luabind.hpp>

#include "sp/utils/Log.h"
#include "sp/system/VFS.h"

#include "AudioScript.h"

namespace sp { namespace scripting {

	lua_State* Scripting::CreateState(lua_State** state)
	{
		lua_State* s = luaL_newstate();
		*state = s;
		return s;
	}

	void Scripting::Init(lua_State* state)
	{
		luaopen_io(state);
		luaopen_base(state);
		luaopen_table(state);
		luaopen_string(state);
		luaopen_math(state);

		luaL_openlibs(state);

		luabind::open(state);
	}

	void Scripting::LoadFile(lua_State* state, const char* filename)
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

	void Scripting::Register(lua_State* state, const char* functionname, lua_CFunction functionpointer)
	{
		lua_register(state, functionname, functionpointer);
	}

	bool Scripting::Call(lua_State* state, const char* functionname)
	{
		lua_getglobal(state, functionname);
		if (!lua_isfunction(state, -1)) return true;

		lua_pcall(state, 0, 0, 0);
		return false;
	}

	void Scripting::CallPreInternal(lua_State* state, const char* functionname) {
		lua_getglobal(state, functionname);
	}

	void Scripting::CallAfterInternal(lua_State* state, int nargs) {
		lua_pcall(state, nargs, 0, 0);
	}

	void Scripting::LoadSparkyAPI(lua_State* state)
	{
		audio::L_Register_Audio_Classes(state);
	}
} }