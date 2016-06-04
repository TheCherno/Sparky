#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#endif

#include <sp\utils\Log.h>

namespace sp { namespace scripting {

namespace internal {

	template <typename First>
	void PassParameter(lua_State* L, int& index, First&& first) {
		Marshal::Dispatch(L, first);
		index++;
	}

	template <typename First, typename... Args>
	void PassParameter(lua_State* L, int& index, First&& first, Args&&... args) {
		Marshal::Dispatch(L, first);
		index++;
		if (sizeof...(Args))
			PassParameter(L, index, std::forward<Args>(args)...);
	}
}

struct FunctionCaller
{
	template <typename... Args>
	static void Dispatch(lua_State* L, const char* func, Args&&... args) {
		int index = 0;
		int top = lua_gettop(L);
		lua_getglobal(L, func);

		if (sizeof...(Args))
			internal::PassParameter(L, index, std::forward<Args>(args)...);

		if (lua_pcall(L, index, 0, 0) != 0) {
			std::string s(lua_tostring(L, -1));
			SP_ERROR("Lua: There was an error running a function. \'", func, "\'");
			SP_ERROR("	", s);
			return;
		}
		return;
	}
};

} }