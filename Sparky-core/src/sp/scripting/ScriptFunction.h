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

	void PushValue(lua_State* L, lua_Integer n);
	void PushValue(lua_State* L, lua_Number n);
	void PushValue(lua_State* L, bool b);
	void PushValue(lua_State* L, const std::string& s);

} }