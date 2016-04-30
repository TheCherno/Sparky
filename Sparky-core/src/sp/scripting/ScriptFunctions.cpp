#include "sp/sp.h"
#include "ScriptFunction.h"

#include <lua.hpp>

#include "sp/utils/Log.h"

using namespace sp;

namespace sp { namespace scripting {

	void ScriptFunctions::PushValue(lua_State* L, int32& position, const std::string& s)
	{
		lua_pushstring(L, s.c_str());
		position++;
	}

	void ScriptFunctions::PushValue(lua_State* L, int32& position, const char* s)
	{
		lua_pushstring(L, s); 
		position++;
	}

	void ScriptFunctions::PushValue(lua_State* L, int32& position, lua_Integer n)
	{
		lua_pushinteger(L, n);
		position++;
	}

	void ScriptFunctions::PushValue(lua_State* L, int32& position, lua_Number n)
	{
		lua_pushnumber(L, n);
		position++;
	}

	void ScriptFunctions::PushValue(lua_State* L, int32& position, bool b)
	{
		lua_pushboolean(L, b);
		position++;
	}

} }