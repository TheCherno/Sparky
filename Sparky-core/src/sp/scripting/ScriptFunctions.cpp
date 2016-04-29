#include "sp/sp.h"
#include "ScriptFunction.h"

#include <lua.hpp>

#include "sp/utils/Log.h"

using namespace sp;

namespace sp { namespace scripting {

	void PushValue(lua_State* L, lua_Integer n)
	{
		lua_pushinteger(L, n);
	}

	void PushValue(lua_State* L, lua_Number n)
	{
		lua_pushnumber(L, n);
	}

	void PushValue(lua_State* L, bool b)
	{
		lua_pushboolean(L, b);
	}

	void PushValue(lua_State* L, const std::string& s)
	{
		lua_pushstring(L, s.c_str());
	}

} }