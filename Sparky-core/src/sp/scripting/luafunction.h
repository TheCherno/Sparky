#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

template<class T, typename... Args>
struct FunctionWrapper
{

	template<T method, typename R>
	static int Dispatch(lua_State* L)
	{
		int index = 1;
		T* obj = luaW_check<T>(L, index++);
		Marshal::Dispatch(L, (obj->*method)(Unmarshal<Args>::Dispatch(L, index++)...));
		return 1;
	}

	template<T method>
	static int Dispatch(lua_State* L)
	{
		int index = 1;
		T* obj = luaW_check<T>(L, index++);
		(obj->*method)(Unmarshal<Args>::Dispatch(L, index++)...);
		return 0;
	}
};

template<class T, typename... Args>
struct StaticFunctionWrapper
{

	template<T method, typename R>
	static int Dispatch(lua_State* L)
	{
		int index = 1;
		Marshal::Dispatch(L, (*method)(Unmarshal<Args>::Dispatch(L, index++)...));
		return 1;
	}

	template<T method>
	static int Dispatch(lua_State* L)
	{
		int index = 1;
		(*method)(Unmarshal<Args>::Dispatch(L, index++)...);
		return 0;
	}
};