#pragma once

#include <string>

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

#ifdef _WIN64
	typedef unsigned __int64 size_t;
	typedef __int64          ptrdiff_t;
	typedef __int64          intptr_t;
#else
	typedef unsigned int     size_t;
	typedef int              ptrdiff_t;
	typedef int              intptr_t;
#endif

typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

namespace sp { namespace scripting {

	class SP_API ScriptFunctions {
	public:
		static void PushValue(lua_State* L, int32& position, const std::string& s);
		static void PushValue(lua_State* L, int32& position, const char* s);
		static void PushValue(lua_State* L, int32& position, lua_Integer n);
		static void PushValue(lua_State* L, int32& position, lua_Number n);
		static void PushValue(lua_State* L, int32& position, bool b);
	};

} }