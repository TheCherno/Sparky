#pragma once

struct lua_State; 
typedef int(*lua_CFunction) (lua_State *L);

namespace sp { namespace scripting {

	class SP_API Scripting {
	private:
		lua_State* m_ScriptState;
	public:
		static lua_State* CreateState(lua_State** state);
		static void Init(lua_State* state);

		static void LoadSparkyAPI(lua_State* state);

		static void LoadFile(lua_State* state, const char* filename);
		
		static void Register(lua_State* state, const char* functionname, lua_CFunction functionpointer);
		static bool Call(lua_State* state, const char* functionname);

		template <typename T1>
		static bool Call(lua_State* state, const char* functionname, const T1& p1);

		template <typename T1, typename T2>
		static bool Call(lua_State* state, const char* functionname, const T1& p1, const T2& p2);

		template <typename T1, typename T2, typename T3>
		static bool Call(lua_State* state, const char* functionname, const T1& p1, const T2& p2, const T3& p3);

		template <typename T1, typename T2, typename T3, typename T4>
		static bool Call(lua_State* state, const char* functionname, const T1& p1, const T2& p2, const T3& p3, const T4& p4);
	};
} }

