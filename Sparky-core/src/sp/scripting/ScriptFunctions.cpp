#include "sp/sp.h"
#include "ScriptFunction.h"

#include <lua.hpp>

#include "sp/utils/Log.h"

using namespace sp;

namespace sp { namespace scripting {

	void ScriptFunctions::PushValue(lua_State* L, const std::string& s)
	{
		lua_pushstring(L, s.c_str());
	}

	void ScriptFunctions::PushValue(lua_State* L, const char* s)
	{
		lua_pushstring(L, s);
	}

	void ScriptFunctions::PushValue(lua_State* L, lua_Integer n)
	{
		lua_pushinteger(L, n);
	}

	void ScriptFunctions::PushValue(lua_State* L, lua_Number n)
	{
		lua_pushnumber(L, n);
	}

	void ScriptFunctions::PushValue(lua_State* L, bool b)
	{
		lua_pushboolean(L, b);
	}

} }