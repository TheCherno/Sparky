#pragma once

struct lua_State;
struct luaL_Reg;

namespace sp { namespace scripting { namespace entity {

	int L_Register_Entity_Classes(lua_State *L);

} } }
