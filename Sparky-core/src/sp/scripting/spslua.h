#pragma once

#ifndef LUAMANGO
#define LUAMANGO

#include <string>
#include <assert.h> 
#include <iostream>
#include <vector>

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

#define LUAM_METHOD(T, S) luaL_Reg(#S, WrapMethod<decltype(&T::S), &T::S>())
#define LUAM_CONSTRUCTOR(S, ...) sp::scripting::WrapConstructor<S, __VA_ARGS__>

namespace sp {
	namespace scripting {
		lua_State* state;
	}
}

template <class T, typename... Args>
void RegisterClass(lua_State* L, const char* classname, std::vector<luaL_Reg> table, std::vector<luaL_Reg> metatable, T* (*allocator)(lua_State*) = luaW_defaultallocator<T>)
{
	table.push_back({ NULL, NULL });
	metatable.push_back({ NULL, NULL });
	luaW_register<T>(L, classname, table.data(), metatable.data(), allocator);
}

#define LUAM_CLASSREGISTERNOCONS(L, S, T, MT) RegisterClass<S>(L, #S, T, MT, nullptr)
#define LUAM_CLASSREGISTER(L, S, T, MT, C) RegisterClass<S>(L, #S, T, MT, C)
#define LUAM_CALLFUNCTION(L, F, ...) sp::scripting::CallFunction(L, F, __VA_ARGS__)

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

#define LuaState sp::scripting::state

#define LUAM_NEWSTATE(L) L = luaL_newstate()
#define LUAM_INIT(L) InitLua(L)
#define LUAM_LOADFILE(L, F) LoadLuaFile(L, F)

#include "API.h"
#define LUAM_LOADAPI(L) sp::scripting::Load(L)

#endif