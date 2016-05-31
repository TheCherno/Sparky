#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#endif

#include <sp\utils\Log.h>

namespace sp { namespace scripting {

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

template <typename R, typename... Args>
R CallFunction(lua_State* L, const char* func, Args&&... args) {
	int index = 0;
	lua_getglobal(L, func);
	PassParameter(L, index, std::forward<Args>(args)...);
	if (lua_pcall(L, index, 0, 0) != 0) {
		std::string s(lua_tostring(L, -1));
		SP_ERROR("Lua: error running function \'", func, "\': ", s);
	}
	return Unmarshal<R>::Dispatch(L, index + 1);
}

template <typename R>
R CallFunction(lua_State* L, const char* func) {
	int index = 0;
	lua_getglobal(L, func);
	if (lua_pcall(L, index, 0, 0) != 0) {
		std::string s(lua_tostring(L, -1));
		SP_ERROR("Lua: error running function \'", func, "\': ", s);
	}
	return Unmarshal<R>::Dispatch(L, index + 1);
}

} }