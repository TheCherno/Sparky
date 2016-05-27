#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

template<class T>
struct Unmarshal
{
	static T Dispatch(lua_State* L, int index)
	{
		return *luaW_check<T>(L, index);
	}
};


template<>
struct Unmarshal<double>
{
	static double Dispatch(lua_State* L, int index)
	{
		return (double)luaL_checknumber(L, index);
	}
};

template<>
struct Unmarshal<float>
{
	static float Dispatch(lua_State* L, int index)
	{
		return (float)luaL_checknumber(L, index);
	}
};

template<>
struct Unmarshal<int>
{
	static int Dispatch(lua_State* L, int index)
	{
		return (int)luaL_checknumber(L, index);
	}
};

template<>
struct Unmarshal<char*>
{
	static const char* Dispatch(lua_State* L, int index)
	{
		return luaL_checkstring(L, index);
	}
};

template<>
struct Unmarshal<std::string>
{
	static std::string Dispatch(lua_State* L, int index)
	{
		return std::string(luaL_checkstring(L, index));
	}
};