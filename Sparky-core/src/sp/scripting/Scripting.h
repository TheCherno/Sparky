#pragma once

#include "ScriptFunction.h"

namespace sp { namespace scripting {

	class SP_API Scripting {
	private:
		lua_State* m_ScriptState;
	public:
		Scripting() {}
		~Scripting() {}

		static lua_State* CreateState(lua_State** state);
		static void Init(lua_State* state);

		static void LoadSparkyAPI(lua_State* state);

		static void LoadFile(lua_State* state, const char* filename);
		
		static void Register(lua_State* state, const char* functionname, lua_CFunction functionpointer);
		static bool Call(lua_State* state, const char* functionname);
	private:
		static void CallPreInternal(lua_State* state, const char* functionname);
		static void CallAfterInternal(lua_State* state, int nargs);

		template <typename First>
		static void Call(lua_State* state, int32& position, First&& first)
		{
			const char* formatted = sp::internal::to_string<First>(first);
			ScriptFunctions::PushValue(state, position, formatted);
		}

		template <typename First, typename... Args>
		static void Call(lua_State* state, int32& position, First&& first, Args&&... args)
		{
			const char* formatted = sp::internal::to_string<First>(first);
			ScriptFunctions::PushValue(state, position, formatted);
			if (sizeof...(Args))
				Call(state, position, std::forward<Args>(args)...);
		}
	public:
		template <typename... Args>
		static void Call(lua_State* state, const char* functionname, Args... args) {
			int32 position = 0;
			CallPreInternal(state, functionname);
			Call(state, position, std::forward<Args>(args)...);
			CallAfterInternal(state, position);
		}
	};
} }

