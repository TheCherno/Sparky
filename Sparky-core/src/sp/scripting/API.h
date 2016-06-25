#pragma once

typedef struct lua_State lua_State;

namespace sp { namespace scripting {
	void Load(lua_State* L);
} }