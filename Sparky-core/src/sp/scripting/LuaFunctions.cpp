#include "sp/sp.h"
#include "LuaFunctions.h"

#include "sp/utils/Log.h"

using namespace sp;

namespace sp { namespace scripting {

	bool LuaFunctions::Call(lua_State* state, const char* functionname)
	{
		lua_getglobal(state, functionname);
		if (!lua_isfunction(state, -1)) return true;

		lua_pcall(state, 0, 0, 0);
		return false;
	}

	void LuaFunctions::CallPreInternal(lua_State* state, const char* functionname)
	{
		lua_getglobal(state, functionname);
	}

	void LuaFunctions::CallAfterInternal(lua_State* state, int nargs)
	{
		lua_pcall(state, nargs, 0, 0);
	}

} }