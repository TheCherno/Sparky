#pragma once

#ifndef LUAMANGO
#define LUAMANGO

#include <string>
#include <assert.h> 
#include <iostream>
#include <vector>
#include <map>

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

#include "marshal.h"
#include "unmarshal.h"
#include "luaconstructor.h"
#include "luacall.h"
#include "luafunction.h"

namespace sp { namespace scripting {
	
	lua_State* state;

} }
#define LuaState sp::scripting::state

namespace sp { namespace scripting {
	std::map<const char*, std::vector<luaL_Reg>> locFunctions;
	std::map<const char*, std::vector<luaL_Reg>> staFunctions;

	template <class T, typename... Args>
	void RegisterClass(lua_State* L, const char* classname, T* (*allocator)(lua_State*) = luaW_defaultallocator<T>)
	{
		staFunctions[classname].push_back({ NULL, NULL });
		locFunctions[classname].push_back({ NULL, NULL });
		luaW_register<T>(L, classname, staFunctions[classname].data(), locFunctions[classname].data(), allocator);
	}

	template<typename T, T method>
	void RegisterLocalFunction(const char* cname, const char* fname) {
		locFunctions[cname].push_back(luaL_Reg(fname, WrapMethod<T, method>()));
	}

	template<typename T, T method>
	void RegisterStaticFunction(const char* cname, const char* fname) {
		staFunctions[cname].push_back(luaL_Reg(fname, WrapMethod<T, method>()));
	}

	void InitLua(lua_State* L)
	{
		luaopen_io(L);
		luaopen_base(L);
		luaopen_table(L);
		luaopen_string(L);
		luaopen_math(L);
		luaL_openlibs(L);
	}

	void LoadLuaFile(lua_State* L, const char* file)
	{
		luaL_dofile(L, file);
		int errors = lua_pcall(L, 0, LUA_MULTRET, 0);
		if (errors != 0)
		{
			std::string s(lua_tostring(L, -1));
			std::cout << "Lua >> " << s << std::endl;
			lua_pop(L, 1);
		}
	}
} }

#define LUAM_FUNCTION(N, T, S) sp::scripting::RegisterLocalFunction<decltype(&N::T::S), &N::T::S>(#T, #S)
#define LUAM_STATICFUNCTION(N, T, S) sp::scripting::RegisterStaticFunction<decltype(&N::T::S), &N::T::S>(#T, #S)
#define LUAM_CONSTRUCTOR(S, ...) sp::scripting::WrapConstructor<S, __VA_ARGS__>

#define LUAM_NEWSTATE() LuaState = luaL_newstate()
#define LUAM_INIT() sp::scripting::InitLua(LuaState)
#define LUAM_LOADFILE(F) sp::scripting::LoadLuaFile(LuaState, F)

#define _LUAM_CLASSREGISTER(N, S) sp::scripting::RegisterClass<N::S>(LuaState, #S, nullptr)
#define LUAM_CLASSREGISTER(N, S, ...) sp::scripting::RegisterClass<N::S>(LuaState, #S, LUAM_CONSTRUCTOR(N::S, __VA_ARGS__))
#define _LUAM_CALLFUNCTION(F, ...) sp::scripting::FunctionCaller<void>::Dispatch(LuaState, F, __VA_ARGS__)
#define LUAM_CALLFUNCTION(F, R, ...) sp::scripting::FunctionCaller<R>::Dispatch(LuaState, F, __VA_ARGS__)

#include "API.h"
#define LUAM_LOADAPI() sp::scripting::Load(LuaState)

#endif