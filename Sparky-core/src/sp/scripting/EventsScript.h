#pragma once

struct lua_State;
struct luaL_Reg;

namespace sp { namespace scripting { namespace events {

	int L_Register_Events_Classes(lua_State *L);

} } }
