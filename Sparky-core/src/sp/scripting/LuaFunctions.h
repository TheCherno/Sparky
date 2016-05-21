#pragma once

#include "sp/sp.h"

#include <string>

#ifdef LUABRIDGE
#include <luabind\luabind.hpp>
#else
#ifndef LUA_LOADED
#define LUA_LOADED
#include <lua.hpp>
#include "luabridge\LuaBridge.h"
#endif
#endif

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

namespace sp { namespace scripting {

	class SP_API LuaFunctions
	{
	public:
		static void Call(lua_State* state, const char* functionname)
		{
			LuaRef func = getGlobal(state, functionname);
			func();
		}

		template <typename T>
		static T Call(lua_State* state, const char* functionname)
		{
			LuaRef func = getGlobal(state, functionname);
			return func<T>();
		}

		template <typename... Args>
		static void Call(lua_State* state, const char* functionname, Args... args)
		{
			try
			{
				LuaRef func = getGlobal(state, functionname);
				func(std::forward<Args>(args)...);
			}
			catch (LuaException e)
			{
				SP_ERROR("LuaException >> ", e.what());
			}
		}

		template <typename T, typename... Args>
		static T Call(lua_State* state, const char* functionname, Args... args)
		{
			LuaRef func = getGlobal(state, functionname);
			return func<T>(std::forward<Args>(args)...);
		}
	};

} }