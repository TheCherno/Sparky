#pragma once

#include "sp/sp.h"

#include <string>

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

namespace sp { namespace scripting {

	class SP_API LuaFunctions
	{
	public:
		static void PushValue(lua_State* L, int& position, const std::string& s);
		static void PushValue(lua_State* L, int& position, const char* s);
		static void PushValue(lua_State* L, int& position, lua_Integer n);
		static void PushValue(lua_State* L, int& position, lua_Number n);
		static void PushValue(lua_State* L, int& position, bool b);

		static bool Call(lua_State* state, const char* functionname);
	private:
		static void CallPreInternal(lua_State* state, const char* functionname);
		static void CallAfterInternal(lua_State* state, int nargs);

		template <typename First>
		static void Call(lua_State* state, int32& position, First&& first)
		{
			const char* formatted = sp::internal::to_string<First>(first);
			PushValue(state, position, formatted);
		}

		template <typename First, typename... Args>
		static void Call(lua_State* state, int32& position, First&& first, Args&&... args)
		{
			const char* formatted = sp::internal::to_string<First>(first);
			PushValue(state, position, formatted);
			if (sizeof...(Args))
				Call(state, position, std::forward<Args>(args)...);
		}
	public:
		template <typename... Args>
		static void Call(lua_State* state, const char* functionname, Args... args)
		{
			int32 position = 0;
			CallPreInternal(state, functionname);
			Call(state, position, std::forward<Args>(args)...);
			CallAfterInternal(state, position);
		}
	};

} }