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
	};
} }

