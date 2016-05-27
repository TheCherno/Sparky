#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

struct Marshal
{
	static int Dispatch(lua_State* L, const double& value)
	{
		lua_pushnumber(L, value);
		return 1;
	}

	static int Dispatch(lua_State* L, const float& value)
	{
		lua_pushnumber(L, value);
		return 1;
	}

	static int Dispatch(lua_State* L, const int& value)
	{
		lua_pushnumber(L, value);
		return 1;
	}

	static int Dispatch(lua_State* L, const char* value)
	{
		lua_pushstring(L, value);
		return 1;
	}

	static int Dispatch(lua_State* L, const std::string& value)
	{
		lua_pushstring(L, value.c_str());
		return 1;
	}

	template<class T>
	static int Dispatch(lua_State* L, T* value)
	{
		luaW_push<T>(L, value);
		return 1;
	}

	template<class T>
	static int Dispatch(lua_State* L, T value)
	{
		luaW_push<T>(L, &value);
		return 1;
	}
};