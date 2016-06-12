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

template<class T, typename P1, typename P2, typename P3, typename P4>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1), Unmarshal<P2>::Dispatch(L, 2), Unmarshal<P3>::Dispatch(L, 3), Unmarshal<P4>::Dispatch(L, 4));
}

template<class T, typename P1, typename P2, typename P3, typename P4, typename P5>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1), Unmarshal<P2>::Dispatch(L, 2), Unmarshal<P3>::Dispatch(L, 3), Unmarshal<P4>::Dispatch(L, 4), Unmarshal<P5>::Dispatch(L, 5));
}

template<class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1), Unmarshal<P2>::Dispatch(L, 2), Unmarshal<P3>::Dispatch(L, 3), Unmarshal<P4>::Dispatch(L, 4), Unmarshal<P5>::Dispatch(L, 5), Unmarshal<P6>::Dispatch(L, 6));
}

template<class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
T* WrapConstructor(lua_State* L)
{
	return new T(Unmarshal<P1>::Dispatch(L, 1), Unmarshal<P2>::Dispatch(L, 2), Unmarshal<P3>::Dispatch(L, 3), Unmarshal<P4>::Dispatch(L, 4), Unmarshal<P5>::Dispatch(L, 5), Unmarshal<P6>::Dispatch(L, 6), Unmarshal<P7>::Dispatch(L, 7));
}

} }