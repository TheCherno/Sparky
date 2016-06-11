#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

namespace sp { namespace scripting {

template<class T>
T* WrapConstructor(lua_State* L)
{
	return new T();
}

template<class T, typename... Args>
T* WrapConstructor(lua_State* L)
{
	int index;
	return new T(Unmarshal<Args>::Dispatch(L, index++)...);
}

} }