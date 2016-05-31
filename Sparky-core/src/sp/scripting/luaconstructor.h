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

template<class T, typename P1>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1));
}

template<class T, typename P1, typename P2>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1), Unmarshal<P2>::Dispatch(L, 2));
}

template<class T, typename P1, typename P2, typename P3>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1), Unmarshal<P2>::Dispatch(L, 2), Unmarshal<P3>::Dispatch(L, 3));
}

} }