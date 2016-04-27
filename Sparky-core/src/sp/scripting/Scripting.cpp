#include "sp/sp.h"
#include "Scripting.h"

#include <lua.hpp>

#include "sp/utils/Log.h"
#include "sp/system/VFS.h"

#include "AudioScript.h"
#include "MathsScript.h"
#include "EntityScript.h"
#include "EventsScript.h"

namespace sp { namespace scripting {

	void RepErrors(lua_State* state, int e) {
		if (e != 0) {
			const char* error = lua_tostring(state, -1);
			lua_pop(state, 1);
			SP_ERROR("LUA Error: ", error);
		}
	}
	
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
	}

	void Scripting::LoadFile(lua_State* state, const char* filename)
	{
		String physicalPath;
		if (VFS::Get()->ResolvePhysicalPath(filename, physicalPath))
		{
			luaL_loadfile(state, physicalPath.c_str());
			int s = lua_pcall(state, 0, LUA_MULTRET, 0);
			RepErrors(state, s);
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

	void Scripting::LoadSparkyAPI(lua_State* state)
	{
		audio::L_Register_Audio_Classes(state);
		maths::L_Register_Math_Classes(state);
		entity::L_Register_Entity_Classes(state);
		events::L_Register_Events_Classes(state);
	}
} }