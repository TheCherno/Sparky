#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

template<typename T>
struct FunctionWrapper
{
};

template<typename T, typename R, typename... Args>
struct FunctionWrapper<R(T::*)(Args...)>
{
	enum { ArgCount = sizeof...(Args) };

	typedef R(T::* FunctionType)(Args...);

	template<FunctionType method, size_t... index>
	static lua_CFunction WrapMethod()
	{
		return [](lua_State *L)
		{
			T* obj = luaW_check<T>(L, 1);
			Marshal::Dispatch(L, (obj->*method)(Unmarshal<Args>::Dispatch(L, index + 2)...));
			return 1;
		};
	}

	template<FunctionType method, size_t... index>
	static lua_CFunction WrapMethodInternal(std::index_sequence<index...>)
	{
		return WrapMethod<method, index...>();
	}
};

template<typename T, typename R, typename... Args>
struct FunctionWrapper<R(T::*)(Args...) const>
{
	enum { ArgCount = sizeof...(Args) };

	typedef R(T::* FunctionType)(Args...) const;
	typedef R(T::* FunctionTypeNonConst)(Args...);
	template <FunctionType method, size_t... index> static lua_CFunction WrapMethod()
	{
		return FunctionWrapper<FunctionTypeNonConst>::WrapMethod<(FunctionTypeNonConst)method, index...>();
	}
	template <FunctionType method, size_t... index> static lua_CFunction WrapMethodInternal(std::index_sequence<index...>)
	{
		return FunctionWrapper<FunctionTypeNonConst>::WrapMethod<(FunctionTypeNonConst)method, index...>();
	}
};

template<typename T, typename... Args>
struct FunctionWrapper<void(T::*)(Args...)>
{
	enum { ArgCount = sizeof...(Args) };
	typedef void (T::* FunctionType)(Args...);

	template<FunctionType method, size_t... index>
	static lua_CFunction WrapMethod()
	{
		return [](lua_State *L)
		{
			T* obj = luaW_check<T>(L, 1);
			(obj->*method)(Unmarshal<Args>::Dispatch(L, index + 2)...);
			return 0;
		};
	}

	template<FunctionType method, size_t... index>
	static lua_CFunction WrapMethodInternal(std::index_sequence<index...>)
	{
		return WrapMethod<method, index...>();
	}
};

template<typename T, T method, std::size_t N, typename Indices = std::make_index_sequence<N>>
lua_CFunction WrapMethod()
{
	return FunctionWrapper<T>::WrapMethodInternal<method>(Indices());
}

template<typename T, T method>
lua_CFunction WrapMethod()
{
	return WrapMethod<T, method, FunctionWrapper<T>::ArgCount>();
}