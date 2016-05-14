#include "sp/sp.h"
#include "LuaFunctions.h"

#include <lua.hpp>

#include "sp/utils/Log.h"

using namespace sp;

namespace sp { namespace scripting {

	void LuaFunctions::PushValue(lua_State* L, int32& position, const std::string& s)
	{
		lua_pushstring(L, s.c_str());
		position++;
	}

	void LuaFunctions::PushValue(lua_State* L, int32& position, const char* s)
	{
		lua_pushstring(L, s); 
		position++;
	}

	void LuaFunctions::PushValue(lua_State* L, int32& position, lua_Integer n)
	{
		lua_pushinteger(L, n);
		position++;
	}

	void LuaFunctions::PushValue(lua_State* L, int32& position, lua_Number n)
	{
		lua_pushnumber(L, n);
		position++;
	}

	void LuaFunctions::PushValue(lua_State* L, int32& position, bool b)
	{
		lua_pushboolean(L, b);
		position++;
	}


	bool LuaFunctions::Call(lua_State* state, const char* functionname)
	{
		lua_getglobal(state, functionname);
		if (!lua_isfunction(state, -1)) return true;

		lua_pcall(state, 0, 0, 0);
		return false;
	}

	void LuaFunctions::CallPreInternal(lua_State* state, const char* functionname) {
		lua_getglobal(state, functionname);
	}

	void LuaFunctions::CallAfterInternal(lua_State* state, int nargs) {
		lua_pcall(state, nargs, 0, 0);
	}

} }