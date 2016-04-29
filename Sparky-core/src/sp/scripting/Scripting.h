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
		static void CallAfterInternal(lua_State* state, int args);
	public:
		template <typename T1>
		static void Call(lua_State* state, const char* functionname, T1* p1) {
			CallPreInternal(state, functionname);
			ScriptFunctions::PushValue(state, p1);
			CallAfterInternal(state, 1);
		}

		template <typename T1, typename T2>
		static void Call(lua_State* state, const char* functionname, T1* p1, T2* p2) {
			CallPreInternal(state, functionname);
			ScriptFunctions::PushValue(state, p1);
			ScriptFunctions::PushValue(state, p2);
			CallAfterInternal(state, 2);
		}

		template <typename T1, typename T2, typename T3>
		static void Call(lua_State* state, const char* functionname, T1* p1, T2* p2, T3* p3) {
			CallPreInternal(state, functionname);
			ScriptFunctions::PushValue(state, p1);
			ScriptFunctions::PushValue(state, p2);
			ScriptFunctions::PushValue(state, p3);
			CallAfterInternal(state, 3);
		}

		template <typename T1, typename T2, typename T3, typename T4>
		static void Call(lua_State* state, const char* functionname, T1* p1, T2* p2, T3* p3, T4* p4) {
			CallPreInternal(state, functionname);
			ScriptFunctions::PushValue(state, p1);
			ScriptFunctions::PushValue(state, p2);
			ScriptFunctions::PushValue(state, p3);
			ScriptFunctions::PushValue(state, p4);
			CallAfterInternal(state, 4);
		}
	};
} }

